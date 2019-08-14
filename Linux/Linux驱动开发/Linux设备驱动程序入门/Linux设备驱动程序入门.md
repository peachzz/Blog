
----------

#### Linux设备驱动程序入门 ####

5/17/2019 11 :09 :48 AM 

Author:terrycoder

----------


#### 驱动程序加载方式 ####
驱动程序加载方式有两种，一种是直接编译进linux内核，在linux启动时候加载，另一种是采用内核模块方式，这种模块可动态加载与卸载。

内核配置中< >带头的行是内核模块配置，[ ]带头的行是内核功能的配置。

在使用 make zImage 命令编译内核时所有的设置为<*>的项将被包含在内核映像中。
在使用make modules命令编译内核时，所有设置为<M>的项将被编译，编译结束后可以使用如下命令安装内核中可加载模块文件到一个指定的目录：

	make modules_install INSTALL_MOD_PATH=/home/user/modules

使用make命令编译内核相当于执行make zImage 和make modules

#### 编写可加载模块 ####

Linux内核模块必须包含以下两个接口

	module_init(init_func);//模块初始化接口
	mudule_exit(exit_func);//模块卸载接口

加载一个内核模块

	#insmod modulesname.ko

卸载一个内核模块

	#rmmod modulename

如果可加载模块源代码独立于内核代码树，需要为可加载模块编写makefile文件

	CC=arm-none-linux-gnueabi-gcc
	obj-m:=smodule.o
	KERNELDIR ?=/root/fgj/linux-4.5.2

CC是编译器，obj-m为需要编译的目标模块，KERNELDIR为内核路径。KERNELDIR的内核版本必须与运行的内核版本一致，否则无法加载编译内核。
	
	static int demo_module_init(void){
		printk("module init\n");
		return 0;
	} 
	
	static void demo_module_exit(void){
		printk("module exit\n");
	}
	
	module_init(demo_module_init);
	module_exit(demo_module_exit);
	
	MODULE_DESCRIPTION("simple module");
	MODULE_LICENSE("GPL");

编写一个Makefile文件如下：

	AR=ar
	ARCH=arm
	CC=arm-none-linux-gnueabi-gcc
	DEBFILAGS=-O2
	obj-m:=smodule.o
	KERNELDIR ?=/root/fgj/linux-4.5.2
	PWD:=$(shell pwd)
	modules:
		$(MAKE)-C $(KERNELDIR) M=$(PWD)LDDINC=$(PWD)/../include modules
	clean:
		rm -rf *.o *~ core.depen.*.cmd *.ko *.mod.c .tmp_versions

#### 带参数的可加载模块 ####

宏MODULE_PARM(var,type,right)用于向模块传递命令行参数。参数类型可以是整数，长整型，字符串等类型。
其中var 为变量值，type为变量类型，right为默认值

	static int itype = 0;
	module_param(itype,int,0);
	
	static int btype = 0;
	module_param(btype,bool,0);
	
	static unsigned char ctype = 0;
	module_param(ctype,byte,0);
	
	static char * stype = 0;
	module_param(stype,charp,0);

	static int demo_module_init(void){
		printk("module init\n");
		printk("itype=%d\n",itype);
		printk("btype=%d\n",btype);
		printk("ctype=%d\n",ctype);
		printk("stype='%s'\n",stype);

		return 0;
	} 
	
	static void demo_module_exit(void){
		printk("module exit\n");
	}
	
	module_init(demo_module_init);
	module_exit(demo_module_exit);
	
	MODULE_DESCRIPTION("simple module");
	MODULE_LICENSE("GPL");

执行make后生产smodule.ko

	insmod smodule.ko itype=2 btype=1 cype=0xAC stype='a'

#### 模块依赖 ####

内核模块之间引用函数与变量，必须要导出才能被引用，内核使用宏定义EXPORT_SYMBOL导出变量与函数。一个模块引用其他模块，成为模块依赖关系，被引用的模块必须先安装，引用模块才能安装。模块导出使用宏EXPORT_SYSMBOL

	 //smodule_dep.c
	int function_of_dep(void)
	{
		printk("function_of_dep\n");
		return 0;
	}
	EXPORT_SYSMBOL(function_of_dep);
	//smodule.c
	extern int function_of_dep(void);
	static int __init demo_module_init(void)
	{
		printk("module init \n");
		function_of_dep();
		return 0;
	}

编写Make文件

	AR=ar
	ARCH=arm
	CC=arm-none-linux-gnueabi-gcc
	DEBFILAGS=-O2
	obj-m:=smodule.o smodule_dep.o
	KERNELDIR ?=/root/fgj/linux-4.5.2
	PWD:=$(shell pwd)
	modules:
		$(MAKE)-C $(KERNELDIR) M=$(PWD)LDDINC=$(PWD)/../include modules
	clean:
		rm -rf *.o *~ core.depen.*.cmd *.ko *.mod.c .tmp_versions

必须先安装smodule_dep.ko才能安装smodule.ko

#### 设备驱动程序分类 ####
Linux操作系统主要包含字符设备、块设备、网络设备等三类基本的设备驱动程序。

Linux系统为每个设备分配了一个主设备号与次设备好，主设备号唯一标识了设备类型，次设备好标识具体设备的实例。

每一个字符设备或块设备在文件系统中都有一个设备节点与之对应。在/dev目录下，网络设备可以通过应用层套接字访问网络设备。
/dev 目录下有两个虚拟设备

| 指令 | 作用 |
| --- | --- |
| cat /dev/null > fileName| 清空文件 |
| cat /dev/zero | 填充为0的二进制流 |

字符设备和快设备也可以通过/proc/devices 文件查看

	cat /proc/devices

#### 字符设备驱动原理 ####

**file_operations结构**

**inode_operations结构**

使用register_chrdev注册字符设备

	int register_chrdev(unsigned int major,const char *name,struct file_operations *fops);	
如果major参数为0，则表示采用系统动态分配的主设备号。

使用unregister_chrdev注销字符设备

	int unregister_chrdev(unsigned int major,const char *name);

字符设备模块使用insmod加载，加载完毕需要在/dev目录下使用mkmod命令建立相应的文件节点。

**使用cdev_add注册字符设备**

**cdev结构**

使用以下一组函数来对cdev结构进行操作：


	struct cdev * cdev_alloc(void); //分配一个cdev
	void cdev_init(struct cdev*,const struct file_operations *);//初始化cdev
	void cdev_put(struct cdev *p);	//减少使用计数
	int cdev_add(struct cdev *p,dev_t dev,unsigned count);//注册设备，通常发生在驱动模式的加载函数中
	void cdev_del(struct cdev *p); //注销设备，通常发生在驱动模块的卸载函数中

使用cdev_add注册字符设备前应先调用register_chrdev_region或者alloc_chrdev_region分配设备号。
register_chrdev_region 用于指定设备号，alloc_chrdev_region用于动态申请设备号。

int register_chrdev_region(dev_t from,unsigned count,const char *name);
int alloc_chrdev_region(dev_t * dev,unsigned baseminor,unsigned count,const char *name);

其中register_chrdev_region申请从from开始count个主版本号，alloc_chrdev_region申请一个动态主版本号，和baseminor为起始的次版本号，count为次版本号数量。



