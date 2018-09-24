
ifneq ($(KERNELRELEASE),)
	obj-m := tl_dbg.o
	tl_dbg-objs := tl_kernel.o
else
	PWD := $(shell pwd)
	KDIR := /lib/modules/$(shell uname -r)/build

all:build-krn build-usr

build-krn:
	$(MAKE) -C $(KDIR) M=$(PWD)

build-usr:
	gcc -o tl_user tl_user.c

insmod:
	sudo insmod tl_dbg.ko

rmmod:
	sudo rmmod tl_dbg

clean:
	rm -f tl_user
	rm -f .cache.mk .*.cmd
	rm -f *.o *.o.cmd *.ko *.mod.c *.symvers *.order
	rm -rf .tmp_versions
endif