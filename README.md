SCG002B i2c-lcd driver for RPi
====
## Description
![usage](https://github.com/fujimotok/SCG002B/blob/img/img/usage.png)

"SCG002B" i2c-lcd module's driver.  
Sorry... Only validated in raspberry-pi 2.  
Not validated other environment.  

[Datasheet](http://doc.switch-science.com/datasheets/SCG002BUTB.pdf)

## Requirement
It uses i2c client driver linux/i2c.h .  
Pleas Enable i2c config.  
```shell-session: 
$ sudo raspi-config
```
Advanced Options > I2C > Yes  

## Usage
- print upper row  
`$ echo -n "hello" > /sys/bus/i2c/drivers/SCG002B/xxx/lcd_row0`
- print lower row  
`$ echo -n "World" > /sys/bus/i2c/drivers/SCG002B/xxx/lcd_row1`
- clear all row  
`$ echo -n "1" > /sys/bus/i2c/drivers/SCG002B/xxx/lcd_clear`

## Setup
![circuit](https://github.com/fujimotok/SCG002B/blob/img/img/circuit.png)

## Install
deploy kernel module.
```shell-session: 
mkdir -p /lib/modules/`uname -r`/misc
cp SCG002B.ko /lib/modules/`uname -r`/misc
depmod -a
```
enable i2c device.
```shell-session: 
echo "SCG002B 0x3a" > /sys/bus/i2c/devices/i2c-X/new_driver
```
## Uninstall
disable i2c device.  
```shell-session: 
echo "SCG002B" > /sys/bus/i2c/devices/i2c-X/delete_driver
```
unload kernel module.  
```shell-session: 
rmmod SCG002B
```

## Author
[fujimotok](https://github.com/fujimotok)
