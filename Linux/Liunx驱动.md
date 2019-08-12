----------

2019/7/16 22 :36 :29 

Linux驱动开发

Author :terry

----------


linux驱动

Makefile文件

编译Hello World 模块需要编写一个Makefile文件

	ifeq ($(KERNELRELEASE),)
		KERNELDIR ?= /linux-2.6.29.4/linux-2.6.29.4
		PWD := $(shell pwd)
	modules:
		$(MAKE) -C $(KERNELDIR) M=$ (PWD) modules
	modules_install:
		$(MAKE) -C $(KERNELDIR) M=$ (PWD) modules_install
	clean:
		rm -rf *.o *~core .depend .*.cmd *.ko *.mod.c .tmp_versions
	else
		obj-m := hello.o
	endif
	