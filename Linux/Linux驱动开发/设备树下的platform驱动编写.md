
----------

#### 设备树下的platform驱动编写 ####

10/22/2019 3 :48 :38 PM 

Author：terrycoder

----------

#### 设备树下的platform驱动编写 ####

**设备树下的platform驱动简介**

platform驱动框架分为总线、设备和驱动，其中总线不需要驱动程序员去管理，由Linux内核提供，我们只需要在编写的时候关注**设备**和**驱动**的具体实现即可。

- 在未使用设备树的Linux内核下，我们需要分别编写并注册platform_device和platform_driver,分别代表设备和驱动。
- 在使用设备树的时候，设备的描述被放到设备树中，**因此platform_device就不需要我们去编写了，我们只需要实现platform_driver即可。**

在编写基于设备树的platform驱动的时候我们需要注意以下几点：

- 在设备树中创建设备节点
- 编写platform驱动时候要注意兼容属性
- 编写platform驱动

**在设备树中创建设备节点**

首先要在设备树中创建设备几点来描述设备信息，重点是设置好compatible属性的值，**因为platform总线需要通过设备节点的compatible属性值来匹配驱动。**比如，编写如下设备节点来描述LED这个设备：

	gpioled{
		#address-cells = <1>;
		#size-cells = <1>;
		compatible = "firefly-gpioled";
		pinctrl-names = "default";
		pinctrl-0 = <&pinctrl_led>;
		led-gpio = <&gpio1 3 GPIO_ACTIVE_LOW>;
		status = "okay";
	}

在编写platform驱动的时候要设置of_match_table也有compatible的属性值"firefly-gpioled"。

**编写platform驱动时候要注意兼容属性**

在使用设备树的时候platform驱动会通过of_match_table来保存兼容性值，也就是表明此驱动兼容哪些设备。比如LEDplatform驱动中platform_driver可以按照如下所示设置：

	//兼容属性表，每一个元素都是一个兼容属性，表示兼容设备。一个驱动可以跟多个设备匹配。
	static const strcut of_device_id leds_of_match[] = {
		{.compatible = "firefly-gpioled"},  /*与设备树中compatible匹配*/
		{/* Sentinel */}
	};
	
	MODULE_DEVICE_TABLE(of,leds_of_match); /*声明leds_of_match设备匹配表*/
	
	static struct platform_driver leds_platform_driver = {
		.driver = {
			.name = "firefly-led",
			.of_match_table = leds_of_match,
		},
		.probe = leds_probe,
		.remove = leds_remove,
	};

驱动中的compatible属性和设备中的compatible属性相匹配，因此驱动中对应的probe函数就会执行。注意{/\* Sentinel\*/}中的空元素，**在编写of\_device\_id的时候最后一个元素一定要为空。**

**编写platform驱动**

	#include <linux/types.h>
	#include <linux/kernel.h>
	#include <linux/delay.h>
	#include <linux/ide.h>
	#include <linux/init.h>
	#include <linux/module.h>
	#include <linux/errno.h>
	#include <linux/gpio.h>
	#include <linux/cdev.h>
	#include <linux/device.h>
	#include <linux/of_gpio.h>
	#include <linux/semaphore.h>
	#include <linux/timer.h>
	#include <linux/irq.h>
	#include <linux/wait.h>
	#include <linux/poll.h>
	#include <linux/fs.h>
	#include <linux/fcntl.h>
	#include <linux/platform_device.h>
	#include <asm/mach/map.h>
	#include <asm/uaccess.h>
	#include <asm/io.h>
	
	
	#define LEDDEV_CNT	1
	#define LEDDEV_NAME "dtsplatled"
	#define LEDOFF		0
	#define LEDON		1
	
	struct leddev_dev{
		dev_t	devid;				/*设备号*/
		struct cdev cdev;			  /*cdev*/
		struct class *class;		   /*类*/
		struct device *device;		 /*设备*/
		int		major;			  /*主设备号*/
		struct device_node *node;	  /*LED设备节点*/
		int		led0;			   /*LED灯GPIO标号*/
	};
	
	struct leddev_dev leddev;
	
	void led0_switch(u8 sta)
	{
		if(sta == LEDON)
			gpio_set_value(leddev.led0,0);
		else if(sta == LEDOFF)
			gpio_set_value(leddev.led0,1);
	}
	
	/*打开设备*/
	static int led_open(struct inode *inode,struct file *filp)
	{
		filp->private_data = &leddev; /*设置私有数据*/
		return 0;
	}
	
	/*向设备写数据*/
	static ssize_t led_write(struct file *filp,const char __user *buf,size_t cnt,loff_t *offt)
	{
		int retvalue;
		unsigned char databuf[2];
		unsigned char ledstat;
	
		retvalue = copy_from_user(databuf,buf,cnt);
		if(retvalue < 0){
			printk("kernel write failed!\r\n");
			return -EFAULT;
		}
	
		ledstat = databuf[0];
		if(ledstat == LEDON){
			led0_switch(LEDON);
		}else if(ledstat == LEDOFF){
			led0_switch(LEDOFF);
		}
		return 0 ;
	}

	static struct file_operations led_fops = {
		.owner = THIS_MODULE,
		.open = led_open,
		.write = led_write,
	};
	
	/*flatform驱动，当驱动与设备匹配以后此函数会执行*/
	static int led_probe(struct platform_device *dev)
	{
		printk("led driver and device was matched \r\n");
	
		/*1.设置设备号*/
		if(leddev.major){
			leddev.devid = MKDEV(leddev.major,0);
			register_chrdev_region(leddev.devid,LEDDEV_CNT,LEDDEV_NAME);
		}else{
			alloc_chrdev_region(&leddev.devid,0,LEDDEV_CNT,LEDDEV_NAME);
			leddev.major = MAJOR(leddev.devid);
		}
	
		/*2.注册设备*/
		cdev_init(&leddev.cdev,&led_fops);
		cdev_add(&leddev.cdev,leddev.devid,LEDDEV_CNT);
		
		/*3.创建类*/
		leddev.class = class_create(THIS_MODULE,LEDDEV_NAME);
		if(IS_ERR(leddev.class)){
			return PTR_ERR(leddev.class);
		}

		/*4.创建设备*/
		leddev.device = device_create(leddev.class,NULL,leddev.devid,NULL,LEDDEV_NAME);
		
		if(IS_ERR(leddev.device){
			return PTR_ERR(leddev.device);
		}
	
		/*5初始化IO*/
		leddev.node = of_find_node_by_path("/gpioled");
		if(leddev.node == NULL){
			printk("gpioled node nost find!\r\n");
			return -EINVAL;
		}
	
		leddev.led0 = of_get_named_gpio(leddev.node,"led-gpio",0);
		if(leddev.led0 < 0){
			printk("cant't get led-gpio\r\n");
			return -EINVAL;
		}
	
		gpio_request(leddev.led0,"led0");
		gpio_direction_output(leddev.led0,1);
		return 0;
	}

	static int led_remove(struct platform_device *dev)
	{
		gpio_set_value(leddev.led0,1);
		cdev_del(&leddev.cdev);
		unregister_chrdev_region(leddev.devid,LEDDEV_CNT);
		device_destroy(leddev.class,leddev.devid);
		class_destroy(leddev.class);
		return 0; 
	}

	/*匹配列表*/
	static const struct of_device_id led_of_match[] = {
		{ .compatible = "firefly-gpioled"},
		{}
	};
	
	/*platform 驱动结构体*/
	static struct platform_driver led_driver = {
		.driver = {
			.name = "firefly-led",			/*驱动名字，用于设备匹配*/
			.of_match_table = led_of_match,  /*设备树匹配表*/
		},
		.probe = led_probe,
		.remove = led_remove,
	};

	/*驱动模块加载函数*/
	static int __init leddriver_init(void)
	{
		return platform_driver_register(&led_driver);
	}
	
	/*驱动模块卸载函数*/
	static void __exit leddriver_exit(void)
	{
		platform_driver_unregister(&led_driver);
	}
	
	module_init(leddriver_init);
	module_exit(leddriver_exit);
	
	MODULE_LICENSE("GPL");
	MODULE_AUTHOR("firefly");

驱动模块加载完成以后到/sys/bus/platform/drivers/目录中查看驱动是否存在"firefly-led"。

在/sys/bus/platform/devices/目录下也存在led的设备文件，也就是设备树中的gpioled这个节点。驱动和模块都存在，当驱动和设备匹配成功以后就会调用驱动文件probe回调函数，初始化驱动。
