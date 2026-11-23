ifneq ($(KERNELRELEASE),)
# kbuild part of makefile
obj-m := hello.o
ccflags-y += -g
else
# normal makefile
KDIR ?= $(HOME)/repos/linux-stable

default:
	$(MAKE) -C $(KDIR) M=$$PWD ARCH=arm CROSS_COMPILE="ccache arm-eabi-" modules
	cp hello.ko hello.ko.unstripped
	arm-eabi-strip -g hello.ko

clean:
	$(MAKE) -C $(KDIR) M=$$PWD ARCH=arm CROSS_COMPILE="ccache arm-eabi-" clean
	rm -f *.unstripped

%.s %.i: %.c
	$(MAKE) -C $(KDIR) M=$$PWD ARCH=arm CROSS_COMPILE="ccache arm-eabi-" $@
endif
