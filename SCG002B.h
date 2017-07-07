#ifndef _SCG002B_H_
#define _SCG002B_H_

#define LCD_STARTUP_MSG

#define LCD_INIT_MAGIC          0x30

#define LCD_RS_CMD_WRITE        0x00
#define LCD_RS_DATA_WRITE       0x40
#define LCD_RS_CONTINUE         0x80 // use with data write

#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10 // move view point
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80 // move data pointer

// LCD_ENTRYMODESET
#define LCD_ENTRYSHFT           0x01
#define LCD_ENTRYFREEZE         0x00
#define LCD_ENTRYSHIFTINCREMENT 0x02
#define LCD_ENTRYSHIFTDECREMENT 0x00

// LCD_DISPLAYCONTROL
#define LCD_DISP_ON             0x04
#define LCD_DISP_OFF            0x00
#define LCD_DISP_CURON          0x02
#define LCD_DISP_CUROFF         0x00
#define LCD_DISP_BLINK          0x01
#define LCD_DISP_NOBLINK        0x00

// LCD_CURSORSHIFT
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00

// Functions
#define LCD_FUNC_8BITMODE       0x10
#define LCD_FUNC_4BITMODE       0x00
#define LCD_FUNC_EXTENDS        0x01
#define LCD_FUNC_2LINE          0x04
#define LCD_FUNC_1LINE          0x00
#define LCD_FUNC_5x10DOTS       0x02
#define LCD_FUNC_5x8DOTS        0x00

// EXTENDS only use FNCTIONSET>EXTENDS=1
#define LCD_EXT_WO              0x04 // write order
#define LCD_EXT_WO_L2R          0x02 // left to right
#define LCD_EXT_WO_R2L          0x00
#define LCD_EXT_WO_T2B          0x01 // top to bottom
#define LCD_EXT_WO_B2T          0x00
#define LCD_EXT_ICON            0x08
#define LCD_EXT_ICON_ONLY       0x04
#define LCD_EXT_ICON_WITH       0x00
#define LCD_EXT_ICON_BLINK      0x02
#define LCD_EXT_ICON_NOBLINK    0x00
#define LCD_EXT_TEMP            0x10 // set coefficiant
#define LCD_EXT_TEMP_TC1        0x01 
#define LCD_EXT_TEMP_TC2        0x02
#define LCD_EXT_HVGEN           0x40
#define LCD_EXT_HVGEN_STAGE1    0x01 // s1=1 && s2=1 not allowed
#define LCD_EXT_HVGEN_STAGE2    0x02 
#define LCD_EXT_VLCD            0x80 // use with voltage(x)
#define LCD_EXT_VLCD_VA         0x00
#define LCD_EXT_VLCD_VB         0x40
#define voltage(x)             (x & 0x3F)
#define voltage_upper(x)       ((x >> 4) & 0x03)
#define voltage_lower(x)       (x & 0x0F)

#endif
