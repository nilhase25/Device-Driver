obj-m += HelloWorld.o

KDIR =/lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules

clean:
	make -C $(KDIR) M=$(shell pwd) clean
#
#ifneq ($(KERNELRELEASE),)
#	obj-m := HelloWorld.o
#else
#	KERNELDIR ?= /lib/modules/$(shell uname -r)/build
#	PWD := $(shell pwd)
#
#default:
#	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
#
#endif
