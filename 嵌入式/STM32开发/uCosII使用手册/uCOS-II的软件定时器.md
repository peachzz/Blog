
----------
#### uCOS-II的软件定时器 ####

5/15/2019 5 :23 :42 PM 

Author:terrycoder


----------

#### uCOS-II软件定时器

软件定时器启动之后是由软件定时器任务OSTmr_Task()统一管理

uCOS-II软件定时器的使能位于“os_cfg.h”


```
                                       /* --------------------- TIMER MANAGEMENT --------------------- */
#define OS_TMR_EN                 1    /* Enable (1) or Disable (0) code generation for TIMERS         */
#define OS_TMR_CFG_MAX           16    /*     Maximum number of timers                                 */
#define OS_TMR_CFG_NAME_EN        1    /*     Determine timer names                                    */
#define OS_TMR_CFG_WHEEL_SIZE     8    /*     Size of timer wheel (#Spokes)                            */
#define OS_TMR_CFG_TICKS_PER_SEC 10    /*     Rate at which timer management task runs (Hz)            */
```
#### 创建软件定时器
要使用uCOS-II的软件定时器必须先声明和创建软件定时器，调用OSTmrCreate()函数可以创建一个软件定时器


```
OS_TMR  *OSTmrCreate (INT32U           dly,             //初始定时节拍数
                      INT32U           period,          //周期定时重载节拍数
                      INT8U            opt,             //周期定时和一次性定时选择 OS_TMR_OPT_PERIODIC|OS_TMR_OPT_ONE_SHOT
                      OS_TMR_CALLBACK  callback,        //定时结束后的回调函数
                      void            *callback_arg,    //传给回调函数的参数
                      INT8U           *pname,           //定时器名称
                      INT8U           *perr)            //返回错误类型
```

注意事项 | 说明
-- | --
1 | 创建前必须先为p_tmr声明一个软件定时器对象（OS_TMR）
2 | 创建一次性定时时dly不能为0
3 | 周期性定时时period不能为0
4 | 不可以在中断中调用函数

#### 启动软件定时器
OSTmrStart()函数来启动一个软件定时器
```
BOOLEAN  OSTmrStart (OS_TMR   *ptmr,            //传入一个OS_TMR指针
                     INT8U    *perr)            //返回错误类型   
```

#### 停止软件定时器
可以调用OSTmrStop()函数来停止一个软件定时器
```
BOOLEAN  OSTmrStop (OS_TMR  *ptmr,              //传入一个OS_TMR指针
                    INT8U    opt,               //选项 OS_TMR_OPT_CALLBACK（停止定时器，并执行回调函数）| OS_TMR_OPT_CALLBACK_ARG（停止定时器，并执行回调函数，
                                                //将callback_arg作为新实参）|OS_TMR_OPT_NONE（只停止定时器，不执行指定事件）
                    void    *callback_arg,      //opt为OS_TMR_OPT_CALLBACK_ARG时，作为回调函数的新实参
                    INT8U   *perr)              //返回错误类型
```

#### 删除软件定时器

OSTmrDel()函数用于删除一个软件定时器。

```
BOOLEAN  OSTmrDel (OS_TMR  *ptmr,       //传入一个OS_TMR指针
                   INT8U   *perr)       //返回错误类型  
                   
                                        //返回删除成功或失败
```
**注意事项：不可以在中断中调用该函数**

#### 软件定时器使用实例


```
static void TmrCallback(void *p_tmr, void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;

    CPU_SR_ALLOC();

    printf("%s \r\n", (char *)p_arg);

    cpu_clk_freq = BSP_CPU_ClkFreq();

    LED1_TOGGLE;

    ts_end = OS_TS_GET() - ts_start;

    ts_start = OS_TS_GET();
}

static   OS_TMR      *my_tmr;     //声明软件定时器
void create_tmr(void)、
{
INT8U           err;
my_tmr = OSTmrCreate (     10,
                           10,
                           OS_TMR_OPT_PERIODIC,
                           TmrCallback,
                           (void *)"Time Over!",
                           (INT8U *)"AppTaskTmr",
                           &err);

OSTmrStart(my_tmr, &err);
}
```
