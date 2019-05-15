
----------
#### uCOS-II的任务 ####

5/15/2019 5 :21 :53 PM 

Author:terrycoder


----------

#### uCOS-II任务的内存结构


- 从任务的存储结构上来看，uCOS—II的任务是由三部分组成：

    1.任务程序代码
    
        任务代码就是任务执行的部分

    2.任务堆栈
    
        任务堆栈则用来保存任务的工作环境
        
    3.任务控制块

        任务控制块就是关联了任务代码的程序控制块，它记录了任务的各个属性
    
>**判断线程和进程方法:**
>
>    **具有私有空间的任务就叫做进程，没有私有空间的任务叫做线程**。
    
    
#### uCOS-II的任务分类

- uCOS-II的任务分为系统任务和用户任务两类:
            
    1.系统任务
                
    2.用户任务
    
    
#### uCOS-II的任务状态


任务的状态 | 说明
---|---
睡眠状态 | 任务没有配备任务控制块或者被剥夺了任务控制块的状态
就绪状态 | 任务配备了任务控制块且已经在任务就绪表中进行了就绪登记
运行状态 | 处于就绪状态的任务经过调度器判断获得了CPU使用权，任何时候只有一个任务处于运行状态
等待状态 | 正在运行的任务，需要等待一段时间或者需要一个事件发生再运行时，该任务会把CPU的使用权让给其他任务而进入等待状态
中断服务状态 | 一个正在运行的任务一旦响应中断申请就会中止运行而去执行中断服务程序

- 用户任务代码的一般结构

```
void MyTask(void *pdata)
{
    //任务变量初始化
    
    while(1)
    {
        //可以被中断的用户代码
        OS_ENTER_CRITCAL();
        //不可以被中断的用户代码
        OS_EXIT_CRITCAL();
        //可以被中断的用户代码；
    }
}

```

```
OS_ENTER_CRITCAL(); //进入临界状态，关闭中断

OS_EXIT_CRITCAL(); //退出临界状态，开启中断
```



- 用户应用程序的一般结构


```
//定义用户任务1
void MyTask1(void *pdata)
{
    while(1)
    {
        
    }
}
//定义用户任务2
void MyTask2(void *pdata)
{
    while(1)
    {
        
    }
}
//定义用户任务3
void MyTask3(void *pdata)
{
    while(1)
    {
        
    }
}

void main()
{
    ......
    OS_Init();                      //初始化uCOS-II
    ......
    OSTaskCreate(MyTask1,......);   //创建用户任务1
    OSTaskCreate(MyTask2,......);   //创建用户任务2
    OSTaskCreate(MyTask3,......);   //创建用户任务3
    ......
     OSStart();                     //启动uCOS-II
    ......
}
```

#### 系统任务

- uCOS-II预定义了两个系统任务：空闲任务和统计任务。
    
    - 空闲任务
    
            OSTaskIdle();
        
    - 统计任务
    
            OSStatInit();

        该任务每秒计算一次CPU在单位时间内被使用的时间，并把计算结果以百分比的形式放在变量OSCPUage中以便其他应用程序来了解CPU的利用率。
        
        如果要使用统计任务，需要在系统头文件OS_CFG.H中的系统配置常数OS_TASK_STAT_EN 设置为1，并且在程序中药调用函数OSStatInit()对统计任务进行初始化。
        
#### 任务的优先权和优先级别

优先级别高的任务先运行，优先级别低的任务后运行。
uCOS-II中创建最多64个任务，所有任务的优先级别最多有64级，数字越小，优先级别越高。

在uCOS-II系统配置文件OS_CFG.H中定义了一个用来表示最低优先别的常数OS_LOWEST_PRIO，对应的任务总数不能超过OS_LOWEST_PRIO+1个。

OS_LOWEST_PRIO系统总是把最低优先级别OS_LOWEST_PRIO-1赋给空闲任务，把OS_LOWEST_PRIO-2赋给统计任务
    
- 任务堆栈

    所谓堆栈就是在存储器中按数据“后进先出（LIFO）”的原则组织的连续存储空间。
    
- 任务堆栈的创建

    在OS_CPU.H中有一个数据类型为:OS_STK
    
    在定义任务堆栈的栈区时，只要定义一个OS_STK类型的数据即可。
    

```
#define     TASK_STK_SIZE   512         //定义堆栈的长度512字节
OS_STK_TaskStk[TASK_STK_SIZE];          //定义一个数组来作为任务堆栈
```

        
#### 创建任务函数

```
INT8U  OSTaskCreate (void   (*task)(void *p_arg),   //指向任务指针
                     void    *p_arg,                //传递给任务的参数
                     OS_STK  *ptos,                 //任务堆栈栈顶的指针
                     INT8U    prio)                 //指定任务的优先级别参数

INT8U  OSTaskCreateExt (void   (*task)(void *p_arg),
                        void    *p_arg,
                        OS_STK  *ptos,
                        INT8U    prio,
                        INT16U   id,
                        OS_STK  *pbos,          //任务堆栈栈底的指针
                        INT32U   stk_size,
                        void    *pext,          //指向附加数据域的指针
                        INT16U   opt)           //用于设定操作选项
```


```

#define     MyTaskStk          126
OS_STK_MyTaskStk[MyTaskStk]

void main(void)
{
    ......
    OSTaskCreate(
                MyTask,
                &MyTaskAgu,
                &OS_STK_MyTaskStk[MyTaskStk-1]  //堆栈增长方式向下的处理器
                20
                );
}

OSTaskCreate(
                MyTask,
                &MyTaskAgu,
                &OS_STK_MyTaskStk[0]  //堆栈增长方式向上的处理器
                20
                );
```

#### 任务控制块

用来记录任务的堆栈指针、任务的当前状态、任务的优先级别等一些与任务管理有关的属性的表就叫做任务控制块。

OS_TCB中有一个成员OSTCBStat用来存放任务的当前状态，该成员变量可能的值如下：

值 | 说明
--- | ---
OS_STAT_RDY | 表示任务处于就绪状态
OS_STAT_SEM | 表示任务处于等待信号量状态
OS_STAT_MBOX | 表示任务处于等待消息邮箱状态
OS_STAT_Q | 表示任务处于等待消息队列状态
OS_STAT_SUSPEND | 表示任务处于被挂起状态
OS_STAT_MUTEX | 表示任务处于等待互斥信号量状态

#### 任务控制块链表

在任务控制块的管理上，uCOS-II有两条链表一条空任务块链表（其中所有任务控制哭爱还未分配给任务）和一条任务块链表（其中所有任务控制块已分配给任务），具体做法为：系统调用OSInit()对uCOS-II系统进行初始化时，就先在RAM中建立一个OS_TCB结构类型的数据OSTCBTBl[],然后把个个元素链接成一个链表，从而形成一个空任务块链表。

初始化时建立的空任务链表的元素一共是OS_MAX_TASKS+OS_N_SYS_TASKS个。

```
OS_MAX_TASKS     //在OS_CFG.H 中 表示用户任务的最大数目
OS_N_SYS_TASKS   //在UCOS_II.H中 表示系统任务的数目
```
uCOS-II允许用户应用程序使用函数OSTaskDel()删除一个任务。删除一个任务本质上就是把该任务的任务从任务控制块链表中删掉，并把它归还给空任务控制块链表。

#### 任务就绪表及任务调度

- 任务调度
    
    就是在就绪任务中确定应该马上运行的任务。
        
    - 任务调度器
    
        调度器主要有两个工作：在任务就绪表中查找具有最高优先级别的任务；二是实现任务的切换

```
OS_TASK_SW(); //任务切换
```
        
            
- uCOS-II任务调度器  
    
    uCOS-II有两种任务调度器，一种是任务级调度器，由函数OSSched()来实现，中断级的调度器由函数OSIntExt()来实现
     
```
OSSched();  //任务级任务调度器
        
OSIntExt();  //中断级任务调度器

OSSchedLock(); //给调度器上锁

OSSchedUnlock(); //给调度器解锁
```

      
        
- 任务就绪表
    
    系统总是从处于就绪状态的任务中选择一个任务运行，任务就绪表登记了系统中所有处于就绪状态的任务。
系统对任务就绪表主要有三个操作，登记，注销，查询最高优先级的任务标识。

---

#### 任务的创建

创建任务的工作实质上是创建一个任务控制块，并通过任务控制块把任务代码和任务堆栈关联起来形成一个完整的任务，还要使刚创建的任务进入就绪状态，并接着引发一次任务调度。

- 创建任务的一般方法

```
void main(void)
{
    ......
    OSInit();                           //对uCos-II进行初始化
    ......
    OSTaskCreate(TaskStart,......);     //创建起始任务TaskStart
    OSStart();                          //开始多任务调度
}

void TaskStart(void *pdata)
{
    ......
    //在这安装并启动uCOS-II的时钟
    OSStatInit();                       //初始化统计任务
    ......
    while(1)
    {
        
    }
    
}
```

**需要注意:uCOS-II不允许在中断服务程序中创建任务**

#### 任务的挂起和恢复

OSTaskSuspend()用来挂起除了空闲任务之外的其他任务，用函数OSTaskSuspend()挂起的任务，只能在其他任务中通过调用恢复函数OSTaskResume()使其恢复为就绪状态。

```
OSTaskSuspend()     //挂起任务 在OSTCBStat中做挂起记录，删除除了自身任务外其他任务就绪表中的就绪标志
OSTaskResume()      //恢复任务 清除任务挂起记录，使能任务就绪
```

#### 任务优先级别的修改

在程序运行过程中调用OSTaskChangePrio()来改变任务的优先级别。


```
INT8U  OSTaskChangePrio (
                        INT8U   oldprio,        //任务现在的优先级别
                        INT8U   newprio);        //要修改的优先级别
```

#### 任务的删除

删除一个任务就是把该任务置于睡眠状态，具体做法是，被删除任务的任务控制块从任务控制块链表中删除，并归还给空任务控制块链表，然后在任务就绪表中把该任务的就绪态位置0，于是该任务就不能再被调度器所调用了。

在删除一个占用资源的任务时，一定要谨慎。一般的原则是：提出删除任务请求的任务只负责提出删除任务请求，而删除工作则由被删除任务自己来完成。

可以通过调用OS_TCBDelReq(INT8U prio)函数来发送删除任务请求

    OS_TCBDelReq(INT8U prio);        //发送删除任务请求
    
```
OSTaskDel(OS_PRIO_SELF);     //删除任务自己 OS_PRIO_SELF 为当前任务优先级

//删除任务请求方
while(OS_TCBDelReq(44) != OS_TASK_NOT_EXIT) //删除优先级别为44的任务
{
    OSTimeDly(1);            //延时一个时钟节拍
}

//被删除任务方
if(OSTaskDelReq(OS_PRIO_SELF) == OS_TASK_DEL_REQ)
{
    OSTaskDel(OS_PRIO_SELF);
}else{
    //其他代码
}
```

#### 查询任务的信息

在程序运行中需要了解一个任务的指针、堆栈等信息，这时就可以通过调用OSTaskQuery()来获取选定的任务的信息。查询成功后，得到的任务信息存放在结构OS_TCB类型的变量中。

```
INT8U OSTaskQuery(INT8U prio,
            OS_TCB *Pdata);
```


