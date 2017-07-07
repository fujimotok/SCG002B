HOME ?= /home/who
ARCH := arm
HOST := gcc-linaro-arm-linux-gnueabihf-raspbian
#HOST := gcc-linaro-arm-linux-gnueabihf-raspbian-x64
CROSS_COMPILE := $(HOME)/raspberry-pi/tools/arm-bcm2708/$(HOST)/bin/arm-linux-gnueabihf-
KPATH := $(HOME)/raspberry-pi/linux

obj-m := SCG002B.o

all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KPATH) M=$(PWD) modules

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KPATH) M=$(PWD) clean
