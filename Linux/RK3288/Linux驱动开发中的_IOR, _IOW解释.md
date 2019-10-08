
----------

#### Linux驱动开发中的_IOR, _IOW解释 ####

9/16/2019 3 :32 :18 PM 

Author：terrycoder

----------

#### 前言 ####


在编写ioctl代码之前，需要选择对应不同命令的编号。为了防止对错误的设备使用正确的命令，命令号应该在系统范围内唯一，这种错误匹配并不是不会发生，程序可能发现自己正在试图对FIFO和audio等这类非串行设备输入流修改波特率，如果每一个ioctl命令都是唯一的，应用程序进行这种操作时就会得到一个EINVAL错误，而不是无意间成功地完成了意想不到的操作。

在驱动程序里， ioctl() 函数上传送的变量 cmd 是应用程序用于区别设备驱动程序请求处理内容的值。cmd除了可区别数字外，还包含有助于处理的几种相应信息。 cmd的大小为 32位，共分 4 个域：

    bit31~bit30  2位为 “区别读写” 区，作用是区分是读取命令还是写入命令。
    bit29~bit15  14位为 “数据大小” 区，表示 ioctl() 中的 arg 变量传送的内存大小。
    bit20~bit08  8位为 “魔数”(也称为”幻数”)区，这个值用以与其它设备驱动程序的 ioctl 命令进行区别。
    bit07~bit00  8位为 “区别序号” 区，是区分命令的命令顺序序号。

像命令码中的 “区分读写区” 里的值可能是 _IOC_NONE （0值）表示无数据传输，_IOC_READ (读)， _IOC_WRITE (写) ，_IOC_READ|_IOC_WRITE(双向)。
内核定义了 _IO() , _IOR() , IOW() 和 _IOWR() 这 4 个宏来辅助生成上面的 cmd 。下面分析 _IO() 的实现，其它的类似。


要按Linux内核的约定方法为驱动程序选择ioctl编号，应该首先看看include/asm/ioctl.h和Doucumention/ioctl-number.txt这两个文件。头文件定义了要使用的位字段：类型（幻数）、序数、传送方向以及参数大小等。ioctl-number.txt文件中罗列了内核所使用的幻数，选择自己的幻数要避免和内核冲突。以下是对include/asm/ioctl.h中定义的宏的注释：

	#define         _IOC_NRBITS          8                             //序数（number）字段的字位宽度，8bits

	#define         _IOC_TYPEBITS        8                             //幻数（type）字段的字位宽度，8bits

	#define         _IOC_SIZEBITS       14                             //大小（size）字段的字位宽度，14bits

	#define         _IOC_DIRBITS         2                             //方向（direction）字段的字位宽度，2bits


	#define         _IOC_NRMASK        ((1 << _IOC_NRBITS)-1)    //序数字段的掩码，0x000000FF

	#define         _IOC_TYPEMASK      ((1 << _IOC_TYPEBITS)-1)  //幻数字段的掩码，0x000000FF

	#define         _IOC_SIZEMASK      ((1 << _IOC_SIZEBITS)-1)  //大小字段的掩码，0x00003FFF

	#define         _IOC_DIRMASK       ((1 << _IOC_DIRBITS)-1)   //方向字段的掩码，0x00000003


	#define        _IOC_NRSHIFT        0               				  //序数字段在整个字段中的位移，0

	#define        _IOC_TYPESHIFT      (_IOC_NRSHIFT+_IOC_NRBITS)        //幻数字段的位移，8

	#define        _IOC_SIZESHIFT      (_IOC_TYPESHIFT+_IOC_TYPEBITS)    //大小字段的位移，16

	#define        _IOC_DIRSHIFT       (_IOC_SIZESHIFT+_IOC_SIZEBITS)    //方向字段的位移，30

   

	/*
	
	 * Direction bits.
	
	 */

	#define _IOC_NONE     0U     //没有数据传输
	
	#define _IOC_WRITE    1U     //向设备写入数据，驱动程序必须从用户空间读入数据
	
	#define _IOC_READ     2U     //从设备中读取数据，驱动程序必须向用户空间写入数据


	/*

	*_IOC 宏将dir，type，nr，size四个参数组合成一个cmd参数

	*/

	#define _IOC(dir,type,nr,size) \
	
	       (((dir)  << _IOC_DIRSHIFT) | \
	
	        ((type) << _IOC_TYPESHIFT) | \
	
	        ((nr)   << _IOC_NRSHIFT) | \
	
	        ((size) << _IOC_SIZESHIFT))
	
	   
	
	/*
	
	* used to create numbers 
	
	*/
	
	//构造无参数的命令编号
	
	#define _IO(type,nr)             _IOC(_IOC_NONE,(type),(nr),0) 
	
	//构造从驱动程序中读取数据的命令编号
	
	   
	
	#define _IOR(type,nr,size)     _IOC(_IOC_READ,(type),(nr),sizeof(size)) 
	
	//用于向驱动程序写入数据命令
	
	#define _IOW(type,nr,size)    _IOC(_IOC_WRITE,(type),(nr),sizeof(size))
	
	//用于双向传输
	
	#define _IOWR(type,nr,size) _IOC(_IOC_READ|_IOC_WRITE,(type),(nr),sizeof(size))
	
	/* 
	
	*used to decode ioctl numbers..
	
	 */
	
	//从命令参数中解析出数据方向，即写进还是读出
	
	#define _IOC_DIR(nr)          	(((nr) >> _IOC_DIRSHIFT) & _IOC_DIRMASK)
	
	//从命令参数中解析出幻数type
	
	#define _IOC_TYPE(nr)              (((nr) >> _IOC_TYPESHIFT) & _IOC_TYPEMASK)
	
	//从命令参数中解析出序数number
	
	#define _IOC_NR(nr)           	(((nr) >> _IOC_NRSHIFT) & _IOC_NRMASK)
	
	//从命令参数中解析出用户数据大小
	
	#define _IOC_SIZE(nr)            (((nr) >> _IOC_SIZESHIFT) & _IOC_SIZEMASK)
	
	   
	
	/* …and for the drivers/sound files… */
	
	   
	
	#define IOC_IN            (_IOC_WRITE << _IOC_DIRSHIFT)
	
	#define IOC_OUT         (_IOC_READ << _IOC_DIRSHIFT)
	
	#define IOC_INOUT     	((_IOC_WRITE|_IOC_READ) << _IOC_DIRSHIFT)
	
	#define IOCSIZE_MASK      (_IOC_SIZEMASK << _IOC_SIZESHIFT)
	
	#define IOCSIZE_SHIFT      (_IOC_SIZESHIFT)

以上为linux源码中的定义，在实际应用场景中定义如下：

	#define GPIO_IOC_MAC	'L' 
	#define CMD_ctl485	_IO(GPIO_IOC_MAC,1)
	#define CMD_ctlled	_IOR(GPIO_IOC_MAC,2,int)
	#define CMD_ctlkey	_IOW(GPIO_IOC_MAC,3,int)
	#define CMD_ctl_net_communication	_IOR(GPIO_IOC_MAC,4,int)    // 0: light
	#define CMD_ctl_floor_communication	_IOR(GPIO_IOC_MAC,5,int)  // 0: light
	#define CMD_ctl_screen_power	_IOR(GPIO_IOC_MAC,6,int)       // 1: hight level
	#define CMD_ctl_EMG_LED	_IOR(GPIO_IOC_MAC,7,int)

编写的测试用例：

	#include "stdio.h"
	#include "string.h"
	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;
	
	extern unsigned int __invalid_size_argument_for_IOC;
	#define _IOC_TYPECHECK(t) \
		((sizeof(t) == sizeof(t[1]) && \
		  sizeof(t) < (1 << _IOC_SIZEBITS)) ? \
		  sizeof(t) : __invalid_size_argument_for_IOC)
	
	#define _IOC_NRBITS	8
	#define _IOC_TYPEBITS	8
	
	/*
	 * Let any architecture override either of the following before
	 * including this file.
	 */
	
	#ifndef _IOC_SIZEBITS
	# define _IOC_SIZEBITS	14
	#endif
	
	#ifndef _IOC_DIRBITS
	# define _IOC_DIRBITS	2
	#endif
	
	#ifndef _IOC_NONE
	# define _IOC_NONE	0U
	#endif
	
	#ifndef _IOC_WRITE
	# define _IOC_WRITE	1U
	#endif
	
	#ifndef _IOC_READ
	# define _IOC_READ	2U
	#endif
	
	#define _IOC_NRMASK	((1 << _IOC_NRBITS)-1) //(1 <<8) - 1 = 255 0x000000FF
	#define _IOC_TYPEMASK	((1 << _IOC_TYPEBITS)-1) //255 0x000000FF
	#define _IOC_SIZEMASK	((1 << _IOC_SIZEBITS)-1) // 0x0000003F
	#define _IOC_DIRMASK	((1 << _IOC_DIRBITS)-1)  // 0x00000003
	
	#define _IOC(dir,type,nr,size) \
		(((dir)  << _IOC_DIRSHIFT) | \
		 ((type) << _IOC_TYPESHIFT) | \
		 ((nr)   << _IOC_NRSHIFT) | \
		 ((size) << _IOC_SIZESHIFT))
	
	#define _IOC_NRSHIFT	0
	#define _IOC_TYPESHIFT	(_IOC_NRSHIFT+_IOC_NRBITS)
	#define _IOC_SIZESHIFT	(_IOC_TYPESHIFT+_IOC_TYPEBITS)
	#define _IOC_DIRSHIFT	(_IOC_SIZESHIFT+_IOC_SIZEBITS)
	
	#define _IO(type,nr)		_IOC(_IOC_NONE,(type),(nr),0)
	#define _IOR(type,nr,size)	_IOC(_IOC_READ,(type),(nr),(_IOC_TYPECHECK(size)))
	#define _IOW(type,nr,size)	_IOC(_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
	#define _IOWR(type,nr,size)	_IOC(_IOC_READ|_IOC_WRITE,(type),(nr),(_IOC_TYPECHECK(size)))
	
	#define GPIO_IOC_MAC	'L' 
	#define CMD_ctl485	_IO(GPIO_IOC_MAC,1)
	#define CMD_ctlled	_IOR(GPIO_IOC_MAC,2,int)
	#define CMD_ctlkey	_IOW(GPIO_IOC_MAC,3,int)
	#define CMD_ctl_net_communication	_IOR(GPIO_IOC_MAC,4,int)    // 0: light
	#define CMD_ctl_floor_communication	_IOR(GPIO_IOC_MAC,5,int)  // 0: light
	#define CMD_ctl_screen_power	_IOR(GPIO_IOC_MAC,6,int)       // 1: hight level
	#define CMD_ctl_EMG_LED	_IOR(GPIO_IOC_MAC,7,int)
	
	
	int main(void)
	{
		tdif_msg_device_info_t dev_t;
		
	
		printf("CMD_ctl485:%d\n",CMD_ctl485);
		printf("CMD_ctlled:%d\n",CMD_ctlled);
		printf("CMD_ctlkey:%d\n",CMD_ctlkey);
		printf("CMD_ctl_net_communication:%d\n",CMD_ctl_net_communication);
		printf("CMD_ctl_floor_communication:%d\n",CMD_ctl_floor_communication);
		printf("CMD_ctl_screen_power:%d\n",CMD_ctl_screen_power);
		printf("CMD_ctl_EMG_LED:%d\n",CMD_ctl_EMG_LED);
		
		printf("CMD_ctl485:%08X\n",CMD_ctl485);
		printf("CMD_ctlled:%X\n",CMD_ctlled);
		printf("CMD_ctlkey:%X\n",CMD_ctlkey);
		printf("CMD_ctl_net_communication:%X\n",CMD_ctl_net_communication);
		printf("CMD_ctl_floor_communication:%X\n",CMD_ctl_floor_communication);
		printf("CMD_ctl_screen_power:%X\n",CMD_ctl_screen_power);
		printf("CMD_ctl_EMG_LED:%X\n",CMD_ctl_EMG_LED);
		
		getchar();
		return 0;
	}


打印输出：

	CMD_ctl485:19457
	CMD_ctlled:-2147202046
	CMD_ctlkey:1074023427
	CMD_ctl_net_communication:-2147202044
	CMD_ctl_floor_communication:-2147202043
	CMD_ctl_screen_power:-2147202042
	CMD_ctl_EMG_LED:-2147202041

	CMD_ctl485:00004C01
	CMD_ctlled:80044C02
	CMD_ctlkey:40044C03
	CMD_ctl_net_communication:80044C04
	CMD_ctl_floor_communication:80044C05
	CMD_ctl_screen_power:80044C06
	CMD_ctl_EMG_LED:80044C07

分析可以得出：
	
	#define _IOC(dir,type,nr,size)

	0x00004C01	 从左往右1. 00 为 dir=_IOC_NONE， 2. 00 为 size=0，  3. 4C 为 type=GPIO_IOC_MAC 4. 01 为 nr=1 ；
	0x80044C02	 从左往右1. 80 为 dir=_IOC_READ， 2. 04 为 size=int，3. 4C 为 type=GPIO_IOC_MAC 4. 02 为 nr=2 ；
	0x40044C03	 从左往右1. 40 为 dir=_IOC_WRITE，2. 04 为 size=int，3. 4C 为 type=GPIO_IOC_MAC 4. 03 为 nr=3 ；
	0x80044C04	 从左往右1. 80 为 dir=_IOC_READ， 2. 04 为 size=int，3. 4C 为 type=GPIO_IOC_MAC 4. 03 为 nr=4 ；
	0x80044C05	 从左往右1. 80 为 dir=_IOC_READ， 2. 04 为 size=int，3. 4C 为 type=GPIO_IOC_MAC 4. 03 为 nr=5 ；
	0x80044C06	 从左往右1. 80 为 dir=_IOC_READ， 2. 04 为 size=int，3. 4C 为 type=GPIO_IOC_MAC 4. 03 为 nr=6 ；
	0x80044C07	 从左往右1. 80 为 dir=_IOC_READ， 2. 04 为 size=int，3. 4C 为 type=GPIO_IOC_MAC 4. 03 为 nr=7 ；

