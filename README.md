SCG002B i2c-lcd driver for RPi
====
## Description
"SCG002B" i2c-lcd module's driver.
Sorry... Only validated in raspberry-pi 2.
Not validated other environment.

## Requirement
It uses i2c client driver linux/i2c.h .
Pleas Enable i2c config.
`$ sudo raspi-config`
Advanced Options > I2C > Yes

## Usage
- print upper row
`$ echo -n "hello" > /sys/bus/i2c/drivers/SCG002B/xxx/lcd_row0`
- print lower row
`$ echo -n "World" > /sys/bus/i2c/drivers/SCG002B/xxx/lcd_row1`
- clear all row
`$ echo -n "1" > /sys/bus/i2c/drivers/SCG002B/xxx/lcd_clear`

## Install
deploy kernel module.
    mkdir -p /lib/modules/`uname -r`/misc
    cp SCG002B.ko /lib/modules/`uname -r`/misc
	depmod -a
enable i2c device.
    echo "SCG002B 0x3a" > /sys/bus/i2c/devices/i2c-X/new_driver

## Uninstall
disable i2c device.
    echo "SCG002B" > /sys/bus/i2c/devices/i2c-X/delete_driver
unload kernel module.
    rmmod SCG002B

## Author
[fujimotok](https://github.com/fujimotok)
