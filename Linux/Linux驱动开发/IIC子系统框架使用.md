
----------

#### IIC子系统框架使用 ####

4/8/2020 4 :57 :41 PM 
----------


本章通过IIC子系统软件框架，实现驱动让应用层I2C读取从设备的功能。硬件上，通过I2C主控制器读取mpu6050陀螺仪和加速度数据信息。

#### 简介 ####
I2C（Inter-integrated Circuit）总线支持设备之间的短距离通信，用于处理器和一些外围设备之间的接口，它只需要两根信号线来完成信息交换。I2C最早是飞利浦在1982年开发设计并用于自己的芯片上，一开始只允许100kHz、7-bit标准地址。1992年，I2C的第一个公共规范发行，增加了400kHz的快速模式以及10-bit扩展地址。在I2C的基础上，1995年Intel提出了“System Management Bus” (SMBus)，用于低速设备通信，SMBus把时钟频率限制在10kHz~100kHz，但I2C可以支持0kHz~5MHz的设备：普通模式（100kHz即100kbps）、快速模式（400kHz）、快速模式+（1MHz）、高速模式（3.4MHz）和超高速模式（5MHz）。

I2C总线由一条串行时钟线SCL，一条串行数据线SDA两条线组成。同一时刻总线上有一个主控制器（master）和多个从设备（slave），通过时钟线来进行同步通信。具体协议的时序规则实现本章不作分析（原厂的代码会帮我们实现），重点是理解I2C子系统的框架和驱动编写。

I2C总线传输协议: https://blog.csdn.net/jasonchen_gbd/article/details/77431951


#### I2C子系统软件框架 ####

    i2c driver:从设备驱动层       

    需要和应用层进行交互；封包数据，不知道数据是如何写入到硬件

    ------------------------------------------------------------------------------------
    i2c 核心层：维护i2c 总线，包括i2c driver, i2c client链表        drivers/i2c/i2c-core.c
    
    ------------------------------------------------------------------------------------
    i2c adapter层：i2c控制层，初始化i2c控制器                          drivers/i2c/busses/i2c-s3c2410.c

完成将数据写入或读取-从设备硬件；不知道数据具体是什么，但是知道如何操作从设备类似于平台总线的构架，

- adapter层代码实现iic协议，代码由原厂提供（在内核源码中可以找到s3c2410.c），我们需要向该层提供硬件描述信息（添加到设备树节点）
- core层维护i2c总线，有内核维护（在内核源码中可以找到i2c-core.c），确保adapter层的从设备对象和从设备驱动对象匹配
- 从设备驱动层 ，由我们来实现，用于对上（应用层提供接口）


#### 几个常用的对象 ####
**设备驱动对象**

```
struct i2c_driver {//表示是一个从设备的驱动对象
    int (*probe)(struct i2c_client *, const struct i2c_device_id *);
    int (*remove)(struct i2c_client *);
    struct device_driver driver; //继承了父类
                |
                const struct of_device_id    *of_match_table;
    const struct i2c_device_id *id_table;//用于做比对，非设备树的情况
}

```

**注册和注销**

```
int i2c_add_driver( struct i2c_driver *driver);
void i2c_del_driver(struct i2c_driver *);
```

**i2c client 从设备对象**

	struct i2c_client {//描述一个从设备的信息,不需要在代码中创建，因为是由i2c adapter帮我们创建
	    unsigned short addr;        //从设备地址，来自于设备树中<reg>
	    char name[I2C_NAME_SIZE]; //用于和i2c driver进行匹配，来自于设备树中compatible
	    struct i2c_adapter *adapter;//指向当前从设备所存在的i2c adapter
	    struct device dev;        // 继承了父类
	};

**创建i2c client的函数**

	struct i2c_client *i2c_new_device(struct i2c_adapter *adap, struct i2c_board_info const *info)

**adapter i2c控制器对象**

	struct i2c_adapter {//描述一个i2c控制器，也不是我们要构建，原厂的代码会帮我们构建
	    const struct i2c_algorithm *algo; //算法
	                |
	                int (*master_xfer)(struct i2c_adapter *adap, struct i2c_msg *msgs,int num);
	    struct device dev; //继承了父类，也会被加入到i2c bus
	    int nr; //编号
	}

**注册和注销：**

	int i2c_add_adapter(struct i2c_adapter * adapter);
	void i2c_del_adapter(struct i2c_adapter * adapter);

**数据包对象**

	struct i2c_msg {//描述一个从设备要发送的数据的数据包
	    __u16 addr;     //从设备地址，发送给那个从设备
	    __u16 flags; //读1还是写0
	    __u16 len;        //发送数据的长度
	    __u8 *buf;        //指向数据的指针
	};


**读写数据**

	int i2c_transfer(struct i2c_adapter *adap, struct i2c_msg *msgs, int num)


#### 实现过程 ####

确保i2c core和i2c adatper层必须编译进内核

	make menuconfig
		Device Drivers  --->
			-*- I2C support  ---> //编译i2c-core.c
				I2C Hardware Bus support  --->
					<*> S3C2410 I2C Driver // i2c-s3c2410.c


	[root@farsight i2c-0]# cd /sys/bus/i2c/devices/i2c-0/ 
	[root@farsight i2c-0]# cat name
	s3c2410-i2c

#### 添加设备树节点 ####

根据结合芯片手册，芯片原理图，开发板与原理图的查询，将i2c控制器和从设备的硬件描述添加到设备树节点中(node-value)。

控制器对应的设备树模板信息：arch/arm/boot/dts/exynos4.dtsi。参考模板，将描述从设备信息的设备树节点添加到文件 arch/arm/boot/dts/exynos4412-fs4412.dts


    i2c@138B0000 {/*i2c adapter5信息*/
            #address-cells = <1>;
            #size-cells = <0>;
            samsung,i2c-sda-delay = <100>;
            samsung,i2c-max-bus-freq = <20000>;
            pinctrl-0 = <&i2c5_bus>;
            pinctrl-names = "default";
            status = "okay";

            mpu6050@68 { /*i2c client信息*/
                    compatible = "invensense,mpu6050";
                    reg = <0x68>;
			};
    };

**编写驱动程序 mpu6050_i2c_drv.c**

1. 构建i2c driver，并注册到i2c总线
2. 实现probe方法和remove方法。
	1. 申请设备号，实现fops
	2. 创建设备文件
	3. 通过i2c的接口去初始化i2c从设备

3. 实现fpos接口

最终实现读取从设备（mpu6050）的数据，实际上就是对模块的寄存器操作。由于mpu6050模块支持iic总线，主控制器通过读写iic的方式对mpu6050模块进行读写操作。

首先驱动创建并注册从设备驱动对象到核心层链表，通过设备树节点的compatible属性匹配到设备对象client，然后可以调用i2c_transfer接口对从设备进行读写操作，这里先进行初始化mpu6050模块。然后对上（应用层）提供接口ioctl，让应用程序可以通过ioctl获取mpu6050的三轴角速度和加速度的原始数据。


```
//mpu6050_i2c_drv.c
#include <linux/init.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/slab.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_gpio.h>
#include <linux/i2c.h>
 
#include <asm/io.h>
#include <asm/uaccess.h>
#include "mpu6050.h"
 
 
#define SMPLRT_DIV		0x19 //采样频率寄存器-25 典型值：0x07(125Hz)
									//寄存器集合里的数据根据采样频率更新
#define CONFIG			0x1A	//配置寄存器-26-典型值：0x06(5Hz)
										//DLPF is disabled（DLPF_CFG=0 or 7）
#define GYRO_CONFIG		0x1B//陀螺仪配置-27,可以配置自检和满量程范围
									//典型值：0x18(不自检，2000deg/s)
#define ACCEL_CONFIG		0x1C	//加速度配置-28 可以配置自检和满量程范围及高通滤波频率
										//典型值：0x01(不自检，2G，5Hz)
#define ACCEL_XOUT_H	0x3B //59-65,加速度计测量值 XOUT_H
#define ACCEL_XOUT_L	0x3C  // XOUT_L
#define ACCEL_YOUT_H	0x3D  //YOUT_H
#define ACCEL_YOUT_L	0x3E  //YOUT_L
#define ACCEL_ZOUT_H	0x3F  //ZOUT_H
#define ACCEL_ZOUT_L	0x40 //ZOUT_L---64
#define TEMP_OUT_H		0x41 //温度测量值--65
#define TEMP_OUT_L		0x42
#define GYRO_XOUT_H		0x43 //陀螺仪值--67，采样频率（由寄存器 25 定义）写入到这些寄存器
#define GYRO_XOUT_L		0x44
#define GYRO_YOUT_H		0x45
#define GYRO_YOUT_L		0x46
#define GYRO_ZOUT_H		0x47
#define GYRO_ZOUT_L		0x48 //陀螺仪值--72
#define PWR_MGMT_1		0x6B //电源管理 典型值：0x00(正常启用)
 
 
 
 
int mpu6050_drv_probe(struct i2c_client *client, const struct i2c_device_id *id);//(匹配成功时)probe方法
int mpu5060_drv_remove(struct i2c_client *client);//(结束匹配时)remove方法
 
int mpu6050_write_bytes(struct i2c_client *client, char *buf, int count);
int mpu6050_read_bytes(struct i2c_client *client, char *buf, int count);
int mpu6050_read_reg_byte(struct i2c_client *client, char reg);
 
 
int mpu6050_drv_open(struct inode *inode, struct file *filp);
int mpu6050_drv_close(struct inode *inode, struct file *filp);
long mpu6050_drv_ioctl (struct file *filp, unsigned int cmd, unsigned long args);
 
 
//设计一个全局的设备对象
struct mpu_sensor{
	int dev_major;
	struct device *dev;
	struct class *cls;
	struct i2c_client *client;//记录probe中client
};
 
struct mpu_sensor *mpu_dev;
 
 
const struct of_device_id  of_mpu6050_id[] = {
		{
			.compatible = "invensense,mpu6050",
		},
		{/*northing to be done*/},
 
};
 
const struct i2c_device_id mpu_id_table[] = {
		{"mpu6050_drv", 0x1111},
		{/*northing to be done*/},
};
	
struct i2c_driver mpu6050_drv = {
	.probe = mpu6050_drv_probe,
	.remove = mpu5060_drv_remove,
	.driver = {
		.name = "mpu6050_drv",//随便写，/sys/bus/i2c/driver/mpu6050_drv
		.of_match_table = of_match_ptr(of_mpu6050_id),
	},
	
	.id_table = mpu_id_table,//非设备树情况下的匹配,在设备树的模式下不需要使用
 
};
 
 
 
static int __init mpu6050_i2c_drv_init(void)
{
	// 1，构建i2c driver，并注册到i2c总线
	return i2c_add_driver(&mpu6050_drv);
}
 
static void __exit mpu6050_i2c_drv_exit(void)
{
	i2c_del_driver(&mpu6050_drv);	//注销i2c从驱动对象
}
 
 
 
 
const struct file_operations mpu6050_fops = {
	.open = mpu6050_drv_open,
	.release = mpu6050_drv_close,
	.unlocked_ioctl = mpu6050_drv_ioctl,
 
};
 
 
int mpu6050_drv_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	printk("-----%s----\n", __FUNCTION__);
 
	/*
		申请设备号，实现fops
			创建设备文件
			通过i2c的接口去初始化i2c从设备
	*/
 
	mpu_dev = kzalloc(sizeof(struct mpu_sensor), GFP_KERNEL);
	
	mpu_dev->client = client;
 
	mpu_dev->dev_major = register_chrdev(0,"mpu_drv", &mpu6050_fops);
 
	mpu_dev->cls = class_create(THIS_MODULE, "mpu_cls");
 
	mpu_dev->dev = device_create(mpu_dev->cls, NULL, MKDEV(mpu_dev->dev_major, 0),
				NULL, "mpu_sensor");
	
	char buf1[2] = {PWR_MGMT_1, 0x0};
	mpu6050_write_bytes(mpu_dev->client, buf1, 2);
	
	char buf2[2] = {SMPLRT_DIV, 0x07};
	mpu6050_write_bytes(mpu_dev->client, buf2, 2);
	
	char buf3[2] = {CONFIG, 0x06};
	mpu6050_write_bytes(mpu_dev->client, buf3, 2);
	
	char buf4[2] ={GYRO_CONFIG, 0x18};
	mpu6050_write_bytes(mpu_dev->client, buf4, 2);
	
	char buf5[2] = {ACCEL_CONFIG, 0x01};
	mpu6050_write_bytes(mpu_dev->client, buf5, 2);
 
	return 0;
 
}
 
 
int mpu5060_drv_remove(struct i2c_client *client)
{
	printk("-----%s----\n", __FUNCTION__);
	device_destroy(mpu_dev->cls, MKDEV(mpu_dev->dev_major, 0));
	class_destroy(mpu_dev->cls);
	unregister_chrdev(mpu_dev->dev_major, "mpu_drv");
	kfree(mpu_dev);
	return 0;
}
 
 
 
int mpu6050_write_bytes(struct i2c_client *client, char *buf, int count)
{
 
	int ret;
	struct i2c_adapter *adapter = client->adapter;
	struct i2c_msg msg;
 
	msg.addr = client->addr;
	msg.flags = 0;
	msg.len = count;
	msg.buf = buf;
	
 
	ret = i2c_transfer(adapter, &msg,  1);
 
	return ret==1?count:ret;
 
}
 
int mpu6050_read_bytes(struct i2c_client *client, char *buf, int count)
{
	
		int ret;
		struct i2c_adapter *adapter = client->adapter;
		struct i2c_msg msg;
	
		msg.addr = client->addr;
		msg.flags = I2C_M_RD;
		msg.len = count;
		msg.buf = buf;
		
		ret = i2c_transfer(adapter, &msg,  1);
	
		return ret==1?count:ret;
}
 
//读取某个特定寄存器的地址，然后返回值
int mpu6050_read_reg_byte(struct i2c_client *client, char reg)
{
	// 先写寄存器的地址， 然后在读寄存器的值
 
		int ret;
		struct i2c_adapter *adapter = client->adapter;
		struct i2c_msg msg[2];
 
		char rxbuf[1];
	
		msg[0].addr = client->addr;
		msg[0].flags = 0;
		msg[0].len = 1;
		msg[0].buf = &reg;
 
		msg[1].addr = client->addr;
		msg[1].flags = I2C_M_RD;
		msg[1].len = 1;
		msg[1].buf = rxbuf;
		
		ret = i2c_transfer(adapter, msg,  2);
		if(ret < 0)
		{
			printk("i2c_transfer read error\n");
			return ret;
		}
 
		return rxbuf[0];
 
}
 
int mpu6050_drv_open(struct inode *inode, struct file *filp)
{
	return 0;
}
int mpu6050_drv_close(struct inode *inode, struct file *filp)
{
	return 0;
}
 
long mpu6050_drv_ioctl (struct file *filp, unsigned int cmd, unsigned long args)
{
	union mpu6050_data data;
 
 
	switch(cmd){
		case IOC_GET_ACCEL:
			//读数据
			data.accel.x = mpu6050_read_reg_byte(mpu_dev->client, ACCEL_XOUT_L);
			data.accel.x |= mpu6050_read_reg_byte(mpu_dev->client, ACCEL_XOUT_H) << 8;
 
			data.accel.y = mpu6050_read_reg_byte(mpu_dev->client, ACCEL_YOUT_L);
			data.accel.y |= mpu6050_read_reg_byte(mpu_dev->client, ACCEL_YOUT_H) << 8;
 
			data.accel.z = mpu6050_read_reg_byte(mpu_dev->client, ACCEL_ZOUT_L);
			data.accel.z |= mpu6050_read_reg_byte(mpu_dev->client, ACCEL_ZOUT_H) << 8;
			break;
		case IOC_GET_GYRO:
			data.gyro.x = mpu6050_read_reg_byte(mpu_dev->client, GYRO_XOUT_L);
			data.gyro.x |= mpu6050_read_reg_byte(mpu_dev->client, GYRO_XOUT_H) << 8;
 
 
			data.gyro.y = mpu6050_read_reg_byte(mpu_dev->client, GYRO_YOUT_L);
			data.gyro.y |= mpu6050_read_reg_byte(mpu_dev->client, GYRO_YOUT_H) << 8;
 
			data.gyro.z= mpu6050_read_reg_byte(mpu_dev->client, GYRO_ZOUT_L);
			data.gyro.z |= mpu6050_read_reg_byte(mpu_dev->client, GYRO_ZOUT_H) << 8;
			break;
		case IOC_GET_TEMP:
			data.temp = mpu6050_read_reg_byte(mpu_dev->client, TEMP_OUT_L);
			data.temp |= mpu6050_read_reg_byte(mpu_dev->client, TEMP_OUT_H) << 8;
			break;
		default:
			printk("invalid cmd\n");
			return -EINVAL;
	}
 
	//将所有的数据交给用户
	if(copy_to_user((void __user * )args, &data, sizeof(data)) > 0)
		return -EFAULT;
 
 
	return 0;
}
 
 
module_init(mpu6050_i2c_drv_init);
module_exit(mpu6050_i2c_drv_exit);
MODULE_LICENSE("GPL");
```

**应用程序 mpu6050_test.c**

调用驱动提供的接口，实现对从设备的数据获取

```
//mpu6050_test.c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
 
 
#include "mpu6050.h"
 
 
int main(int argc, char *argv[])
{
	int fd;
	
	union mpu6050_data data;
	
	fd = open("/dev/mpu_sensor", O_RDWR);
	if(fd < 0)
	{
		perror("open");
		exit(1);
	}
 
	while(1)
	{
		ioctl(fd, IOC_GET_ACCEL, &data);
		printf("accel data :  x = %d, y=%d, z=%d\n", data.accel.x, data.accel.y, data.accel.z);
 
	
		ioctl(fd, IOC_GET_GYRO, &data);
		printf("gyro data :  x = %d, y=%d, z=%d\n", data.gyro.x, data.gyro.y, data.gyro.z);
 
		sleep(1);
 
	}
 
	close(fd);
	
 
	return 0;
 
}
```

**应用程序 mpu6050_test.c**

实现了驱动和应用程序共用的命令格式，和数据结构。

```
//mpu6050.h
#ifndef __MPU6050_H__
#define __MPU6050_H__
 
union mpu6050_data{
	struct{	
		short x;
		short y;
		short z;
	}accel;
 
	struct{	
		short x;
		short y;
		short z;
	}gyro;
 
	short temp;
};
 
 
#define IOC_GET_ACCEL  _IOR('M', 0x34,union mpu6050_data)
#define IOC_GET_GYRO  _IOR('M', 0x35,union mpu6050_data)
#define IOC_GET_TEMP  _IOR('M', 0x36,union mpu6050_data)
 
#endif
```