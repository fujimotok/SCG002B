#include <linux/delay.h> 
#include <linux/device.h>
#include <linux/kernel.h> 
#include <linux/mutex.h> 
#include <linux/module.h> 
#include <linux/slab.h> 
#include <linux/i2c.h> 
#include <linux/string.h>
#include "SCG002B.h"

MODULE_DESCRIPTION("SCG002B driver");
MODULE_LICENSE("Dual BSD/GPL");

#define LCD_I2C_ADDR    0x3a
#define LCD_COLS        16

struct SCG002B_device {
  struct i2c_client *client;
};

// register {name, id}
static const struct i2c_device_id SCG002B_id[] = {
  { "SCG002B", 0 },
  { }
};
MODULE_DEVICE_TABLE(i2c, SCG002B_id);

//*** i2c command func *********************************************************************************
static void SCG002B_ascii_remap(char *str)
{
  while (*str) { *str++ += 0x80; }
  return;
}

static int SCG002B_cleardisplay(struct i2c_client *client)  
{
  i2c_smbus_write_byte_data( client, LCD_RS_CMD_WRITE, LCD_CLEARDISPLAY );
  usleep_range(1080, 2000);
        
  return 0;
}

static int SCG002B_puts(struct i2c_client *client, char *str )
{
  SCG002B_ascii_remap(str);
  i2c_smbus_write_i2c_block_data( client, LCD_RS_DATA_WRITE, strlen(str), (unsigned char *)str );
  usleep_range(26,100);
        
  return 0;
}

static int SCG002B_setcursor(struct i2c_client *client, int col, int row )
{
  int row_offs[] = { 0x00, 0x40, 0x14, 0x54 };
        
  i2c_smbus_write_byte_data( client, LCD_RS_CMD_WRITE, LCD_SETDDRAMADDR | (col + row_offs[row]) );
  usleep_range(26,100);
        
  return 0;
}

static int SCG002B_init(struct i2c_client *client)
{
  int ret;
  unsigned char data[8];

  ret = i2c_smbus_write_byte_data(client, LCD_RS_CMD_WRITE, LCD_INIT_MAGIC);
  msleep(5);
  ret = i2c_smbus_write_byte_data(client, LCD_RS_CMD_WRITE, LCD_INIT_MAGIC);
  msleep(5);
  ret = i2c_smbus_write_byte_data(client, LCD_RS_CMD_WRITE, LCD_INIT_MAGIC);
  msleep(5);

  if( ret < 0 ) {
	printk(KERN_ERR "%s: Could not write first function set to i2c lcd device\n", __func__);
	return -ENODEV;
  }

  i2c_smbus_write_byte_data( client, LCD_RS_CMD_WRITE,
							 LCD_FUNCTIONSET | LCD_FUNC_8BITMODE | LCD_FUNC_2LINE | LCD_FUNC_EXTENDS );
        
  data[0] = LCD_EXT_HVGEN | LCD_EXT_HVGEN_STAGE1;
  data[1] = LCD_EXT_VLCD  | LCD_EXT_VLCD_VA | voltage(0x9F);
  data[2] = LCD_EXT_VLCD  | LCD_EXT_VLCD_VB | voltage(0xDF);
        
  ret = i2c_smbus_write_i2c_block_data( client, LCD_RS_CMD_WRITE, 3, data );
  if( ret < 0 ) {
	printk(KERN_ERR "%s: Could not initialize i2c lcd device\n", __func__);
	return -ENODEV;
  }

  i2c_smbus_write_byte_data( client, LCD_RS_CMD_WRITE,
							 LCD_FUNCTIONSET | LCD_FUNC_8BITMODE | LCD_FUNC_2LINE );
  usleep_range(27, 100);
  i2c_smbus_write_byte_data( client, LCD_RS_CMD_WRITE,
							 LCD_DISPLAYCONTROL | LCD_DISP_ON | LCD_DISP_CURON | LCD_DISP_BLINK);
  usleep_range(27, 100);
  i2c_smbus_write_byte_data( client, LCD_RS_CMD_WRITE,
							 LCD_ENTRYMODESET | LCD_ENTRYSHIFTINCREMENT | LCD_ENTRYFREEZE );
  usleep_range(27, 100);

  SCG002B_cleardisplay(client);

#ifdef LCD_STARTUP_MSG
  SCG002B_setcursor(client, 0,0);
  SCG002B_puts(client, "Hello,");
  SCG002B_setcursor(client, 0,1);
  SCG002B_puts(client, "RasPi");
#endif
  
  return 0;
}

//*** device file func **********************************************************************************
static int lcd_row1_store( struct device *dev, struct device_attribute *attr, const char *buf, size_t count )
{
  char str[LCD_COLS+1];
  int size = strlen( buf );
  struct SCG002B_device *data = (struct SCG002B_device *)dev_get_drvdata(dev);
        
  SCG002B_setcursor(data->client, 0, 0 );
  strncpy(str, buf, LCD_COLS );
  str[LCD_COLS] = '\0';
        
  SCG002B_puts(data->client, str);
        
  return size;
}
static DEVICE_ATTR(lcd_row1, S_IWUSR|S_IWGRP, NULL, lcd_row1_store );

static int lcd_row2_store( struct device *dev, struct device_attribute *attr, const char *buf, size_t count )
{
  char str[LCD_COLS+1];
  int size = strlen( buf );
  struct SCG002B_device *data = (struct SCG002B_device *)dev_get_drvdata(dev);
        
  SCG002B_setcursor(data->client, 0, 1 );
  strncpy( str, buf, LCD_COLS );
  str[LCD_COLS] = '\0';
        
  SCG002B_puts(data->client, str);
        
  return size;
}
static DEVICE_ATTR(lcd_row2, S_IWUSR|S_IWGRP, NULL, lcd_row2_store );

static int lcd_clear_store( struct device *dev, struct device_attribute *attr, const char *buf, size_t count)
{
  int size = strlen( buf );
  struct SCG002B_device *data = (struct SCG002B_device *)dev_get_drvdata(dev);
        
  SCG002B_cleardisplay(data->client);
        
  return size;
}
static DEVICE_ATTR(lcd_clear, S_IWUSR|S_IWGRP, NULL, lcd_clear_store );

//*** driver cb func  ***********************************************************************************
static int SCG002B_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
  struct SCG002B_device *dev;
  int ret;
        
  printk( KERN_INFO "SCG002B probing... addr=%d\n", client->addr);

  if (!i2c_check_functionality(client->adapter,
							   I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_I2C_BLOCK)) {
	printk(KERN_ERR "%s: needed i2c functionality is not supported\n", __func__);
	return -ENODEV;
  }

  ret = SCG002B_init(client);

  if(ret) {
	return ret;
  }

  // copy private data
  dev = kzalloc(sizeof(struct SCG002B_device), GFP_KERNEL);
  if (dev == NULL) {
	printk(KERN_ERR "%s: no memory\n", __func__);
	return -ENOMEM;
  }
  dev->client = client;
  i2c_set_clientdata(client, dev);
        
  ret = device_create_file( &client->dev, &dev_attr_lcd_row1 );
  if(ret) {
	printk(KERN_ERR "failed to add lcd_row1 attribute\n" );
  }
        
  ret = device_create_file( &client->dev, &dev_attr_lcd_row2 );
  if(ret) {
	printk(KERN_ERR "failed to add lcd_row2 attribute\n" );
  }
        
  ret = device_create_file( &client->dev, &dev_attr_lcd_clear );
  if(ret) {
	printk(KERN_ERR "failed to add lcd_clear attribute\n" );
  }
        
  return 0;
}

static int SCG002B_remove(struct i2c_client *client)
{
  struct SCG002B_device *dev;
  
  SCG002B_cleardisplay(client);
  printk( KERN_INFO "SCG002B removing ... \n" );
        
  device_remove_file( &client->dev,  &dev_attr_lcd_row1);
  device_remove_file( &client->dev,  &dev_attr_lcd_row2);
  device_remove_file( &client->dev,  &dev_attr_lcd_clear);
        
  dev = (struct SCG002B_device *)i2c_get_clientdata(client);
  kfree(dev);
  return 0;
}

static const unsigned short SCG002B_addr[] = { LCD_I2C_ADDR, I2C_CLIENT_END };

static struct i2c_driver SCG002B_driver = {
  .probe    = SCG002B_probe,
  .remove   = SCG002B_remove,
  .id_table = SCG002B_id,
  .address_list = SCG002B_addr,
  .driver   = {
	.owner = THIS_MODULE,
	.name = "SCG002B",
  },
};

module_i2c_driver(SCG002B_driver);


