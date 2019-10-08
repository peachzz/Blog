----------

#### RK3288_Uboot开发文档 ####

9/9/2019 9 :36 :50 AM 

Author：terrycoder

----------

#### RK3288_Uboot简介 ####
RK Uboot 是基于开源的 Uboot 进行开发的，主要支持：

- 支持芯片： rk3288、 rk3036、 rk312x 等；
- Kernel 和 ramdisk 的加载启动；
- 支持 Rockusb 和 fastboot 两种方式烧写；
- 支持 secure boot 签名加密机制；
- 支持 LVDS、 EDP、 MIPI、 HDMI、 CVBS 等显示设备；
- SDCard、 Emmc、 nand flash 等存储设备；
- 支持开机 logo 显示、充电动画显示，低电管理、电源管理；
- I2C、 SPI、 pmic、 charge、 guage、 usb、 gpio、 pwm、中断等驱动支持;

#### Rockchip 平台相关代码 ####

（1）平台配置文件：

	configs\rk3288_defconfig
	configs\rk3036_defconfig
	configs\rk3126_defconfig
	configs\rk3128_defconfig

RK 芯片平台的配置，主要是芯片类型， RK 一些 Kconfig 的关键配置，采用 savedefconfig 模式保存。

通过执行命令配置：

	make rk3128_defconfig //RK3128 平台的默认配置文件

以 rk3128_defconfig 为例，说明相关配置的含义

	CONFIG_SYS_EXTRA_OPTIONS="RKCHIP_RK3128,SECOND_LEVEL_BOOTLOADER"
	CONFIG_ARM=y
	CONFIG_ROCKCHIP=y
	CONFIG_ROCKCHIP_ARCH32=y

- RKCHIP\_RK3128： 
 定义了 RK3128 的芯片类型， Uboot 解释为 CONFIG\_RKCHIP\_RK3128；
- SECOND\_LEVEL\_BOOTLOADER：
 定义了 Uboot 作为二级 loader 模式，Uboot 解释为CONFIG\_SECOND\_LEVEL\_BOOTLOADER，一般采用 NAND Flash 的项目，需要定义该选
项。 如果 Uboot 作为一级 loader，则不要定义该选项；

我们可以在 Uboot 自动生成的配置文件（include/config.h）中看到生成的宏定义，会优先系统的配置文件，可以支配系统的配置文件。

- CONFIG_ARM=y
说明 ARM 平台
- CONFIG_ROCKCHIP=y
说明 RK 的平台
- CONFIG_ ROCKCHIP_ARCH32=y
说明 RK 芯片系列类型的平台， RK30 系列、 RK32 系列等 32 位芯片。

（2）系统配置文件：

	include\configs\rk_default_config.h
	include\configs\rk30plat.h
	include\configs\rk32plat.h

rk\_default\_config.h： RK 平台的公共配置，默认打开所有的功能。

rk30plat.h/rk32plat.h： RK30/32 系列平台的配置， 根据不同芯片进行一些细节的配置，如内存地址、简配一些功能模块的配置， RK30 系列包含 RK3036、 RK3126、 rRK3128 等芯片 RK32系列包含 RK3288。

**注意：这些文件中不能使用 // 作为注释，会引起 uboot 解析 lds 文件时出错，如果不想定义相关模块，可以直接使用 /\*\*/ 或者 #undef 等。**

#### RK 架构的相关文件 ####

**芯片架构配置目录：**

	configs\rk3288_defconfig
	configs\rk3036_defconfig
	configs\rk3126_defconfig
	configs\rk3128_defconfig
	include\configs\rk30plat.h
	include\configs\rk32plat.h
	include\configs\rk_default_config.h

**芯片架构相关文件目录：**

	arch\arm\include\asm\arch-rk32xx\
	arch\arm\cpu\armv7\rk32xx\
**board 相关文件目录：**

	board\rockchip\
**命令相关文件目录：**

	common\
**驱动相关文件目录：**

	drivers\
**工具相关文件目录：**

	tools\
	tools\rk_tools\

#### 编译 ####

**GCC 工具链的配置**

GCC 的 toolchain，在 uboot 根目录下的 Makefile 中指定：

	make rk3128_defconfig // 编译 rk3128， 如果是 rk3288，则为 make rk3288_defconfig
	make

#### RK Uboot 固件生成 ####

编译完成后生成的镜像：
生成的 uboot 固件如下（包含一级 loader 和 二级 loader 模式）：

	RK3288Loader_uboot_V2.15.01.bin （uboot 作为一级 loader）
	或者
	uboot.img （uboot 作为二级 loader）

其中 V2.15.01 是发布的版本号，这个是 rockchip 定义 uboot loader 的版本， 其中 2.15 是根据flash 版本定义的， 客户务必不要修改这个版本， 01 是 uboot 定义的小版本，用户根据实际需求在Makefile 中修改。

RK3036、 RK3126、 RK3128 采用二级 loader 模式，该模式下只生成一个 uboot.img；
RK3288 可以采用一级和二级两种模式；

- RK3288Loader_uboot_V2.15.01.bin 是一级 loader 模式，只支持 emmc。
- uboot.img 是二级 loader 模式，同时支持 emmc 和 nand flash，二级 loader 模式需要在 rk32plat.h

		#define CONFIG_SECOND_LEVEL_BOOTLOADER

**注意：Uboot 作为二级 loader 时，一级 loader 由 rk 采用 mini loader 单独发出，类似以前的 loader。**

#### Cache 机制 ####

Rockchio 系列芯片 cache 接口 采用 uboot 提供的标准接口，具体可以参考 uboot 的文档，这
里做简单的介绍。

	include\configs\rk32xx.h 中关于 cache 的配置
	/*
	* cache config
	*/
	//#define CONFIG_SYS_ICACHE_OFF
	//#define CONFIG_SYS_DCACHE_OFF
	#define CONFIG_SYS_L2CACHE_OFF
	#define CONFIG_SYS_ARM_CACHE_WRITETHROUGH
	arch\arm\lib\cache-cp15.c 是一些 cache 的使能和关闭函数
	arch\arm\lib\cache.c 是一些 cache 的操作相关的函数
一般情况下 cache 由 rockchip uboot 开发人员维护，请谨慎修改。

#### 中断机制 ####

Rockhip 平台支持标准的 uboot 中断接口函数：

	void enable_interrupts (void);
	int disable_interrupts (void);
	void irq_install_handler(int irq, interrupt_handler_t *handler, void *data);
	void irq_uninstall_handler(int irq);
	int irq_set_irq_type(int irq, unsigned int type);
	int irq_handler_enable(int irq);
	int irq_handler_disable(int irq);
	static inline int gpio_to_irq(unsigned gpio);

#### Clock 驱动 ####

Rk clock 相关代码位于：

	arch\arm\include\asm\arch-rk32xx\clock.h
	arch\arm\cpu\armv7\rk32xx\clock.c
	arch\arm\cpu\armv7\rk32xx\ clock-rk3288.c
	arch\arm\cpu\armv7\rk32xx\ clock-rk3036.c
	arch\arm\cpu\armv7\rk32xx\ clock-rk312x.c

主要的接口函数定义见 arch\arm\include\asm\arch-rk32xx\clock.h，详细请看注释。

#### GPIO 驱动 ####

Rockhip 平台 gpio 接口函数在文件： drivers\gpio\rk_gpio.c 中，RK GPIO 的定义规则如下：

	/*
	* rk gpio api define the gpio format as:
	* using 32 bit for rk gpio value,
	* the high 24bit of gpio is bank id, the low 8bit of gpio is pin number
	* eg: gpio = 0x00010008, it mean gpio1_b0, 0x00010000 is bank id of GPIO_BANK1, 0x00000008 is
	GPIO_B0
	*/
	/* bank and pin bit mask */
	#define RK_GPIO_BANK_MASK 0xFFFFFF00
	#define RK_GPIO_BANK_OFFSET 8
	#define RK_GPIO_PIN_MASK 0x000000FF
	#define RK_GPIO_PIN_OFFSET 0
采用 32bit 定义，高 24bit 为 bank，低 8 位为具体的 pin，如：

	gpio_direction_output(GPIO_BANK7 | GPIO_A4, 1);
具体的 bank 和 pin 在以下相关文件中定义：

	arch\arm\include\asm\arch-rk32xx\gpio.h
	arch\arm\include\asm\arch-rk32xx\gpio-rk3036.h
	arch\arm\include\asm\arch-rk32xx\gpio-rk312X.h
	arch\arm\include\asm\arch-rk32xx\gpio-rk3288.h

####  IOMUX 驱动 ####

Rockchip 平台 gpio 复用功能配置，驱动文件

	arch\arm\include\asm\arch-rk32xx\iomux.h
	arch\arm\cpu\armv7\rk32xx\iomux.c
	arch\arm\cpu\armv7\rk32xx\iomux-rk3036.c
	arch\arm\cpu\armv7\rk32xx\iomux-rk312X.c
	arch\arm\cpu\armv7\rk32xx\iomux-rk3288.c

#### I2C 驱动 ####

Rk I2C 支持标准 uboot 架构，详细可以参考 uboot 文档，相关代码位于：

	drivers\i2c\rk_i2c.c

**注意：目前 i2c 读写最大长度为 32 字节**

#### SPI 驱动 ####

RK SPI 支持标准 uboot 架构，详细可以参考 uboot 文档，相关代码位于：

	drivers\spi\rk_spi.c

#### LCD 驱动 ####

显示模块相关代码如下：

	Drivers/video/rockchip_fb.c
	Drivers/video/rockchip_fb.h
	Drivers/video/rk32_lcdc.c
	Drivers/video/rk3036_lcdc.c

关于屏的电源控制：在 rockhip_fb.c 中， rk_fb_pwr_ctr_prase_dt 会解析 dts 中的所以电源控制接口， rk_fb_pwr_enable/disable 函数分别对显示模块的电源进行开关。 如果屏的上电时序需要重新调整，可以修改该函数。

Uboot 使用的 logo 存放在 kernel 的根目录下，编译时会打包进 resource.img 文件中， Uboot 对
logo 的解析过程请参考 common/lcd.c 中的 rk_bitmap_from_resource（）函数。

开机 Logo 加载流程：
（1）、从 rk_bitmap_from_resource 解析 resource 分区中的 logo；
（2）、如果 resource 分区加载失败， 会从 boot 分区中的 resource 中加载 logo；
（3）、 uboot 的默认 logo 显示流程，所以即使 kernel 目录中不包含 logo.bmp 图片， uboot 也会在屏中间显示一张如下所示的图：
这张名为 rockchip.bmp 的图保存在/u-boot/tools/logos 目录下，为了尽量减小 uboot.bin 的 size，这张图做得很小（只有 200x500,8bit 的 bmp 图片），如果想替换开机 logo，建议修改 kernel 中的logo.bmp 而不是修改 uboot 的 rockchip.bmp。
（4）、如果 resource 分区中存在 logo_kernel.bmp，那么 uboot 会加载该图片到 ddr 中并通过commandline 通知内核加载的位置，内核显示新的 logo 图片。
另外需要强调的是， uboot 对 bmp 的支持比较弱， 目前知道有如下限制：
（1） 只支持偶数分辨率的图片
（2） 所有的 bmp 图片建议用如下命令进行处理后，将处理后的 logo_rle8.bmp 用于显示

	convert -compress rle -colors 256 logo.bmp logo_rle8.bmp
（3） 支持 24bit bmp 开机 logo 显示，支持内核更新一张 24bit logo 显示。
**注意：对于 MID 相关的项目，不要在 uboot 里面打开 HDMI 相关的配置。**

####  emmc 及 nand 的驱动 ####

（1） uboot 支持 emmc 及 nand flash，想要 uboot 同时支持 emmc 和 nand flash,并且开机时自动识别， 如果需要支持 nand flash 定义 CONFIG_SECOND_LEVEL_BOOTLOADER，否则 uboot 只支持emmc。
（2） 存储模块的代码位置在 board/rockchip/common/storage/storage.c 中，初始化入口为int32 StorageInit(void)。如果碰到初始化失败的情况，可注意排查下硬件的焊接情况以及存储设备的支持情况

#### Fastboot ####

Fastboot 是 loader 提供的一种类似 rockusb/adb 的交互模式。 Fastboot 交互中，使用的 PC 工具源码位于 android 源码中（system/core/fastboot/），分为 windows 版本和 linux 版本（windows 端使用的设备驱动与 adb 相同）。

**进入 fastboot 状态的方式**

1、 开机中 loader 启动阶段按键进入（3288sdk 板为 vol-键）：

	checkKey((uint32 *)&boot_rockusb, (uint32 *)&boot_recovery, (uint32 *)&boot_fastboot);
	....
	} else if(boot_fastboot && (vbus!=0)){
	printf("fastboot key pressed.\n");
	frt = FASTBOOT_REBOOT_FASTBOOT;
	}
2、 带有 fastboot 参数的 reboot 命令（reboot fastboot，通过 PMU_SYS_REG0 寄存器传递）

**Fastboot 主要支持命令**

	fastboot getvar version 获得版本
	fastboot getvar version-bootloader 获得版本
	fastboot getvar unlocked 获得解锁情况
	fastboot getvar secure 获得锁住情况（与 unlock 相反）
	fastboot getvar product 获得产品信息
	fastboot getvar serialno 获得序列号
	fastboot getvar partition-type:<partition_name> 获得指定分区类型
	fastboot getvar partition-size:<partition_name> 获得指定分区大小
	fastboot getvar partition-offset:<partition_name> 获得指定分区偏移

**烧写**
- fastboot flash <partition_name> <filename> 烧写固件
（如： fastboot flash system system.img。

烧写 parameter/loader 时，需指定分区名为”parameter”/”loader”）

- fastboot update <filename> 烧写升级包
（升级包通过在 android 源码中 make updatepackage 生成）

**重启**

	fastboot oem recovery 重启进 recovery
	fastboot oem recovery:wipe_data 重启恢复出厂设置
	fastboot reboot 重启
	fastboot reboot-bootloader 重启进入 rockusb 烧写模式
	fastboot continue 重启
** 解锁和锁住设备**

	fastboot oem unlock 解锁
	fastboot oem unlock_accept 确认解锁
	（需要在 fastboot oem unlock 命令后， 5 秒内输入）
	fastboot oem lock 锁住设备
**特殊命令**

	fastboot boot <kernel> [ <ramdisk> ] 临时从指定固件启动
	（kernel 目前支持 Image/zImage，需要将 dtb 存于 kernel 末尾，或者 resource 分区中）
	fastboot oem log 获取串口 log 信息

**Fastboot 解锁**

fastboot 锁住状态下，不允许烧写及执行 oem 命令，初始状态为锁住。

解锁流程大致如下：
1、执行 fastboot oem unlock
2、 5 秒内继续执行 fastboot oem unlock_accept
3、机器会重启进入 recovery 恢复出厂设置
4、再次进入 fastboot，则 fastboot getvar unlocked 应该返回"yes"（设备已解锁）
如果设备进入 fastboot 状态后， fastboot 命令提示未发现设备，则需要在命令中加入-i 参数指定设备 vid，例如 fastboot -i 0x2207 getvar unlocked

**固件加载**

固件加载涉及到 boot、 recovery、 kernel、 resource 分区以及 dtb 文件。

**boot/recovery 分区**

Boot 和 recovery 的固件分为两种形式：

（A）、android 标准格式
标准固件格式将 ramdisk 和 kernel 打包在一起，镜像文件的魔数为”ANDROID!”：

	00000000 41 4E 44 52 4F 49 44 21 24 10 74 00 00 80 40 60 ANDROID!$.t...@`
	00000010 F9 31 CD 00 00 00 00 62 00 00 00 00 00 00 F0 60 .1.....b.......`

标准格式可以带有签名、 checksum 等信息，以及 dtb 文件等额外数据。打包固件时， recovery
镜像默认为标准格式，而标准格式的 boot 镜像则需要通过 ./mkimage.sh ota 方式生成。

（B）、RK 格式
Rk 格式的镜像单独打包一个文件（ramdisk/kernel），镜像文件的魔数为”KRNL”：

	00000000 4B 52 4E 4C 42 97 0F 00 1F 8B 08 00 00 00 00 00 KRNLB...........
	00000010 00 03 A4 BC 0B 78 53 55 D6 37 BE 4F 4E D2 A4 69 .....xSU.7.ON..i
打包生成的 kernel.img、默认打包方式生成的 boot.img 均为 Rk 格式。

**kernel 分区**

Kernel 分区包含 kernel 信息。如果启动时，加载的 boot/recovery 分区自身带有 kernel（android
标准格式），则忽略 kernel 分区，优先使用其自身包含的 kernel。

**resource 分区**

Resource 镜像格式是为了简单存取多个资源文件设计的简易镜像格式，其魔数为”RSCE”：

	00000000 52 53 43 45 00 00 00 00 01 01 01 00 01 00 00 00 RSCE............
	00000010 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 ................
Uboot 支持将 kernel 所需的 dtb 打包在 resource 分区。

**Dtb 文件**

Dtb 文件是新版本 kernel 的 dts 配置文件的二进制化文件。
目前 dtb 文件可以存放于 android 标准格式的 boot/recovery 分区中，也可以存放于 resource 分区。
uboot 假定 kernel 启动必须加载 dtb 文件。

**固件加载流程**

Uboot 加载固件流程为：
1、 加载需要启动的 boot/recovery 分区的 ramdisk 内容
2、 加载启动分区的 kernel 内容。如果失败（为 Rk 格式），则继续加载 kernel 分区
3、 加载启动分区的 dtb 文件。如果失败，则继续尝试从 resource 分区加载。

Dtb 文件（fdt）和 ramdisk 将被加载到 uboot 动态申请的内存中。 Kernel 则被加载到内存 32M 偏移处运行。

#### boot_merger 工具 ####
boot_merger 是用于打包 loader、 ddr bin、 usb plug bin 等文件，生成烧写工具需要的 loader 格式的 linux 版本工具。其源码位于 uboot 源码内：

	uboot# ls ./tools/boot_merger.*
	./tools/boot_merger.c ./tools/boot_merger.h

**支持 loader 的打包和解包**

**打包**

	./tools/boot_merger [--pack] <config.ini>

打包需要传递描述打包参数的 ini 配置文件路径。
（目前使用的配置文件均存放于 uboot 源码内（tools/rk_tools/RKBOOT））如：

	./tools/boot_merger ./tools/rk_tools/RKBOOT/RK3288.ini
	out:RK3288Loader_uboot.bin
	fix opt:RK3288Loader_uboot_V2.15.bin
	merge success(RK3288Loader_uboot_V2.15.bin)

**解包**

	./tools/boot_merger --unpack <loader.bin>

**参数配置文件**
以 3288 的配置文件为例：

	[CHIP_NAME]
	NAME=RK320A ----芯片名称： ” RK” 加上与 maskrom 约定的 4B 芯片型号
	[VERSION]
	MAJOR=2 ----主版本号
	MINOR=15 ----次版本号
	[CODE471_OPTION] ----code471，目前设置为 ddr bin
	NUM=1
	Path1=tools/rk_tools/32_LPDDR2_300MHz_LPDDR3_300MHz_DDR3_300MHz_20140404.b
	in
	[CODE472_OPTION] ----code472，目前设置为 usbplug bin
	NUM=1
	Path1=tools/rk_tools/rk32xxusbplug.bin
	[LOADER_OPTION]
	NUM=2
	LOADER1=FlashData ----flash data，目前设置为 ddr bin
	LOADER2=FlashBoot ----flash boot，目前设置为 uboot bin
	FlashData=tools/rk_tools/32_LPDDR2_300MHz_LPDDR3_300MHz_DDR3_300MHz_201404
	04.bin
	FlashBoot=u-boot.bin
	[OUTPUT] ----输出路径，目前文件名会自动添加版本号
	PATH=RK3288Loader_uboot.bin

**Resource_tool 工具**

resource_tool 是用于打包任意资源文件，生成 resource 镜像的 linux 工具。其源码位于 uboot 源码内（tools/resource_tool/）

**支持 resource 镜像的打包和解包**

**打包**

	./tools/resource_tool [--pack] [--image=<resource.img>] <file list>

如：
	uboot/tools/resource_tool/resources# ../resource_tool `find . -type f`
	Pack to resource.img successed!

pack_resource.sh 脚本可以新增资源文件到现有的镜像：

	./pack_resource <resources dir> <old image> <dst image> <resource_tool path>
如：

	uboot# sudo ./tools/resource_tool/pack_resource.sh
	tools/resource_tool/resources/ ../kernel/resource.img resource.img
	tools/resource_tool/resource_tool
	Pack tools/resource_tool/resources/ & ../kernel/resource.img to
	resource.img ...
	Unpacking old image(../kernel/resource.img):
	rk-kernel.dtb
	Pack to resource.img successed!

	Packed resources:
	rk-kernel.dtb charge_anim_desc.txt images/battery_4.bmp
	images/battery_0.bmp images/battery_1.bmp images/battery_2.bmp
	images/battery_3.bmp images/battery_5.bmp images/battery_fail.bmp
	resource.img

**解包**

	./tools/resource_tool --unpack [--image=<resource.img>] [output dir]