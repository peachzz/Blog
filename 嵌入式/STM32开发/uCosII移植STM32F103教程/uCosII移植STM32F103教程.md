
----------
2019/5/14 21 :40 :46 

#### uCosII移植STM32F103教程 ####

----------

#### 移植环境 ####


1. **本文所使用的移植环境如下：**

	- 硬件环境为：STM32F103ZET6
	- 软件环境为： MDK5.23

准备工作

开始移植前需要准备一个空的工程模板和uCosII官方示例包，这里使用的是Micrium_uC-Eval-STM32F107_uCOS-II.zip版本，解压示例包移动源码：

| 文件名 | 源文件路径 | 移动路径 |
| :--- | :--- |:--- |
| cpu.h、cpu_a.asm、cpu_c.c| \Micrium\Software\uC-CPU\ARM-Cortex-M3\RealView | \Micrium\Software\uC-CPU |
|lib_mem_a.asm |\Micrium\Software\uC-LIB\Ports\ARM-Cortex-M3\RealView|\Micrium\Software\uC-LIB |
|os_cpu.h、os_cpu_a.asm、os_cpu_c.c、os_dbg.c|\Micrium\Software\uCOS-II\Ports\ARM-Cortex-M3\Generic\RealView|\Micrium\Software\uCOS-II\Ports|

![](1.png)

在工程模板下创建uCosII文件夹，将上图三个文件夹复制到uCosII文件夹路径下

打开工程模板，创建如下四个分组uCOS-II_CPU、uCOS-II_LIB、uCOS-II_Ports、uCOS-II_Source：

![](2.png)

分别导入如下路径下的.C和.asm文件（和导入头文件路径也一致）：

|文件路径|
| :--- | 
|\template\uCOS-II\uC-CPU |
|\template\uCOS-II\uC-LIB|
|\template\uCOS-II\uCOS-II\Ports|
|\template\uCOS-II\uCOS-II\Source|

![](3.png)

![](4.png)

![](5.png)

添加完源码工程的所有文件
![](6.png)

![](7.png)

添加头文件路径

![](8.png)

编译工程，提示缺少app_cfg.h文件，该文件在示例包\Micrium\Software\EvalBoards\Micrium\uC-Eval-STM32F107\uCOS-II路径下

![](9.png)

将该路径下的app_cfg.h、app_hooks.c、cpu_cfg.h、includes.h、lib_cfg.h、os_cfg.h文件复制到\template\uCOS-II\OS文件夹下，OS为新建文件夹

将\Micrium\Software\EvalBoards\Micrium\uC-Eval-STM32F107\BSP路径下bsp.c、bsp.h文件复制到\template\uCOS-II\OS文件夹下
![](10.png)

在工程中添加OS分组导入\template\uCOS-II\OS下.c文件，并导入头文件到工程后编译，提示没有stm32f10x_lib.h文件

![](11.png)

将stm32f10x_lib.h替换为stm32f10x.h，后再编译工程

![](12.png)

提示没有bsp_os.h文件,删除除如下代码外的其他所有代码

	void         BSP_Init                    (void);
	CPU_INT32U   BSP_CPU_ClkFreq             (void);

将bsp.c文件中

	static  void  BSP_LED_Init   (void);
	static  void  BSP_StatusInit (void);

	void  BSP_Init (void)函数中代码全部删除
	
	void  BSP_LED_Init (void)
	void  BSP_LED_On (CPU_INT08U led)

	void  BSP_LED_Off (CPU_INT08U led)
	void  BSP_LED_Toggle (CPU_INT08U led)
	static  void  BSP_StatusInit (void)
	CPU_BOOLEAN  BSP_StatusRd (CPU_INT08U  id)代码全部删除

将工程中main.c文件修改为：

	/* Includes ------------------------------------------------------------------*/
	#include <includes.h>
	
	static   OS_STK      AppTaskStartStk[APP_TASK_START_STK_SIZE];
	
	
	static  void    AppTaskCreate  (void);
	static	void	AppEventCreate (void);
	
	static  void    AppTaskStart   (void *p_arg);
	
	int main(void)
	{
	  /* Add your application code here*/
	  CPU_INT08U  err;
	
	
	    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"              */
	
	    OSTaskCreateExt(AppTaskStart,                               /* Create the start task                                    */
	                    (void *)0,
	                    (OS_STK *)&AppTaskStartStk[APP_TASK_START_STK_SIZE - 1],
	                    APP_TASK_START_PRIO,
	                    APP_TASK_START_PRIO,
	                    (OS_STK *)&AppTaskStartStk[0],
	                    APP_TASK_START_STK_SIZE,
	                    (void *)0,
	                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
	
	#if (OS_TASK_NAME_EN > 0)
	    OSTaskNameSet(APP_TASK_START_PRIO, "Start Task", &err);
	#endif
	
	    OSStart();                          
	  /* Infinite loop */
	  while (1)
	  {
	  }
	}
	
	static  void  AppTaskStart (void *p_arg)
	{
	    CPU_INT32U  hclk_freq;
	    CPU_INT32U  cnts;
	
	
	   (void)p_arg;
	
	    BSP_Init();                                                 /* Init BSP fncts.                                          */
	
	    CPU_Init();                                                 /* Init CPU name & int. dis. time measuring fncts.          */
	
	    hclk_freq = BSP_CPU_ClkFreq();                              /* Determine SysTick reference freq.                        */
	    cnts  = hclk_freq / (CPU_INT32U)OS_TICKS_PER_SEC;           /* Determine nbr SysTick increments in OS_TICKS_PER_SEC.    */
	    OS_CPU_SysTickInit(cnts);                                   /* Init uC/OS periodic time src (SysTick).                  */
	
	    Mem_Init();                                                 /* Init Memory Management Module.                           */
	
	#if (OS_TASK_STAT_EN > 0)
	    OSStatInit();                                               /* Determine CPU capacity                                   */
	#endif
	
	#if (APP_CFG_SERIAL_EN == DEF_ENABLED)                          /* Init serial port 0 to 115,2000 baud for use  with  ...   */
	//    BSP_Ser_Init(115200);                                       /* ... tracing. See app_cfg.h.                              */
	#endif
	
	    printf(("Creating Application Events...\n\r"));
	    AppEventCreate();                                           /* Create Application Kernel objects                        */
	
	    printf(("Creating Application Tasks...\n\r"));
	    AppTaskCreate();                                            /* Create application tasks                                 */
	

	    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */   
	        OSTimeDlyHMSM(0, 0, 0, 100);
	    }
	}
	

	static  void  AppTaskCreate (void)
	{

	}
	
	static void AppEventCreate(void)
	{

	}

重新编译
![](14.png)

注意

移植完以后需要将startup_stm32f10x_hd.s文件中以下函数进行修改

|函数名 |修改为 |
|:---| :---|
|PendSV_Handler| OS_CPU_PendSVHandler |
|SysTick_Handler| OS_CPU_SysTickHandler |
![](15.png)
到此uCosII工程移植完毕

添加测试程序，可以正常运行
![](16.png)