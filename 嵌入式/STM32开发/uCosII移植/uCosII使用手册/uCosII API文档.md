uCosII API文档


**Void OSInit(void);**
所属文件 OS_CORE.C    调用者启动代码   开关量无
OSinit（）初始化μC/OS-Ⅱ，对这个函数的调用必须在调用OSStart（）函数之前，而OSStart（）函数真正开始运行多任务。

**Void OSIntEnter(void);**
所属文件 OS_CORE.C    调用者中断     开关量无
OSIntEnter（）通知μC/OS-Ⅱ一个中断处理函数正在执行，这有助于μC/OS-Ⅱ掌握中断嵌套的情况。OSIntEnter（）函数通常和OSIntExit（）函数联合使用。
注意/警告：在任务级不能调用该函数。如果系统使用的处理器能够执行自动的独立执行读取-修改-写入的操作，那么就可以直接递增中断嵌套层数（OSIntNesting），这样可以避免调用函数所带来的额外的开销。

**Void OSIntExit(void);**
所属文件 OS_CORE.C   调用者中断   开关量无
OSIntExit（）通知μC/OS-Ⅱ一个中断服务已执行完毕，这有助于μC/OS-Ⅱ掌握中断嵌套的情况。通常OSIntExit（）和OSIntEnter（）联合使用。当最后一层嵌套的中断执行完毕后，如果有更高优先级的任务准备就绪，μC/OS-Ⅱ会调用任务调度函数，在这种情况下，中断返回到更高优先级的任务而不是被中断了的任务。
注意/警告：在任务级不能调用该函数。并且即使没有调用OSIntEnter（）而是使用直接递增OSIntNesting的方法，也必须调用OSIntExit（）函数。

**Void OSSchedLock(void);**
所属文件 OS_CORE.C   调用者任务或中断  开关量 N/A
OSSchedLock（）函数停止任务调度，只有使用配对的函数OSSchedUnlock（）才能重新开始内核的任务调度。调用OSSchedLock（）函数的任务独占CPU，不管有没有其他高优先级的就绪任务。在这种情况下，中断仍然可以被接受和执行（中断必须允许）。OSSchedLock（）函数和OSSchedUnlock（）函数必须配对使用。μC/OS-Ⅱ可以支持多达254层的OSSchedLock（）函数嵌套，必须调用同样次数的OSSchedUnlock（）函数才能恢复任务调度。
注意/警告:任务调用了OSSchedLock（）函数后，决不能再调用可能导致当前任务挂起的系统函数：OSTimeDly（），OSTimeDlyHMSM（），OSSemPend（），OSMboxPend（），OSQPend（）。因为任务调度已经被禁止，其他任务不能运行，这会导致系统死锁。

**Void OSSchedUnlock(void);**
所属文件 OS_CORE.C   调用者任务或中断  开关量 N/A
在调用了OSSchedLock（）函数后，OSSchedUnlock（）函数恢复任务调度。
注意/警告:任务调用了OSSchedLock（）函数后，决不能再调用可能导致当前任务挂起的系统函数：OSTimeDly（），OSTimeDlyHMSM（），OSSemPend（），OSMboxPend（），OSQPend（）。因为任务调度已经被禁止，其他任务不能运行，这会导致系统死锁。

**void OSStart(void);**
所属文件 OS_CORE.C   调用者初始代码 开关量无
OSStart( )启动μC/OS-II的多任务环境。
注意/警告：在调用OSStart( )之前必须先调用OSInit ( )。在用户程序中OSStart( )只能被调用一次。第二次调用OSStart( )将不进行任何操作。

**void OSStatInit (void);**
所属文件 OS_CORE.C   调用者初始代码 开关量 OS_TASK_STAT_EN &&OS_TASK_CREATE_EXT_EN
OSStatInit（）获取当系统中没有其他任务运行时，32位计数器所能达到的最大值。OSStatInit（）的调用时机是当多任务环境已经启动，且系统中只有一个任务在运行。也就是说，该函数只能在第一个被建立并运行的任务中调用。

**INT8U OSTaskChangePrio (INT8U oldprio, INT8U newprio);**
所属文件 OS_TASK.C   调用者任务 开关量 OS_TASK_CHANGE_PRIO_EN
OSTaskChangePrio（）改变一个任务的优先级。
参数
oldprio是任务原先的优先级。
newprio 是任务的新优先级。
返回值
OSTaskChangePrio（）的返回值为下述之一：
OS_NO_ERR：任务优先级成功改变。
OS_PRO_INVALID：参数中的任务原先优先级或新优先级大于或等于OS_LOWEST_PRIO。
OS_PRIO_EXIST：参数中的新优先级已经存在。
OS_PRIO_ERR：参数中的任务原先优先级不存在。
注意/警告：参数中的新优先级必须是没有使用过的，否则会返回错误码。在OSTaskChangePrio（）中还会先判断要改变优先级的任务是否存在
 
**INT8U OSTaskCreate(void (*task)(void *pd), void *pdata, OS_STK *ptos, INT8U prio);**
所属文件 OS_TASK.C   调用者任务或初始化代码 开关量无
OSTaskCreate（）建立一个新任务。任务的建立可以在多任务环境启动之前，也可以在正在运行的任务中建立。中断处理程序中不能建立任务。一个任务必须为无限循环结构，且不能有返回点。
OSTaskCreate（）是为与先前的μC/OS版本保持兼容，新增的特性在OSTaskCreateExt（）函数中。
无论用户程序中是否产生中断，在初始化任务堆栈时，堆栈的结构必须与CPU中断后寄存器入栈的顺序结构相同。详细说明请参考所用处理器的手册。
参数
task是指向任务代码的指针。
Pdata指向一个数据结构，该结构用来在建立任务时向任务传递参数。
ptos为指向任务堆栈栈顶的指针。任务堆栈用来保存局部变量，函数参数，返回地址以及任务被中断时的CPU寄存器内容。任务堆栈的大小决定于任务的需要及预计的中断嵌套层数。计算堆栈的大小，需要知道任务的局部变量所占的空间，可能产生嵌套调用的函数，及中断嵌套所需空间。如果初始化常量OS_STK_GROWTH设为1，堆栈被设为从内存高地址向低地址增长，此时ptos应该指向任务堆栈空间的最高地址。反之，如果OS_STK_GROWTH设为0，堆栈将从内存的低地址向高地址增长。
prio为任务的优先级。每个任务必须有一个唯一的优先级作为标识。数字越小，优先级越高。
返回值
OSTaskCreate（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_EXIST：具有该优先级的任务已经存在。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
OS_NO_MORE_TCB：系统中没有OS_TCB可以分配给任务了。
注意/警告：任务堆栈必须声明为OS_STK类型。
在任务中必须调用μC/OS提供的下述过程之一：延时等待、任务挂起、等待事件发生（等待信号量，消息邮箱、消息队列），以使其他任务得到CPU。
用户程序中不能使用优先级0，1，2，3，以及OS_LOWEST_PRIO-3, OS_LOWEST_PRIO-2, OS_LOWEST_PRIO-1, OS_LOWEST_PRIO。这些优先级μC/OS系统保留，其余的56个优先级提供给应用程序。

**INT8U OSTaskCreateExt(void (*task)(void *pd), void *pdata, OS_STK *ptos,INT8U prio, INT16U id, OS_STK *pbos, INT32U stk_size, void *pext, INT16U opt);**
所属文件 OS_TASK.C   调用者任务或初始化代码 开关量无
OSTaskCreateExt（）建立一个新任务。与OSTaskCreate（）不同的是，OSTaskCreateExt（）允许用户设置更多的细节内容。任务的建立可以在多任务环境启动之前，也可以在正在运行的任务中建立，但中断处理程序中不能建立新任务。一个任务必须为无限循环结构（如下所示），且不能有返回点。
参数
task是指向任务代码的指针。
Pdata指针指向一个数据结构，该结构用来在建立任务时向任务传递参数。下例中说明μC/OS中的任务代码结构以及如何传递参数pdata：（如果在程序中不使用参数pdata，为了避免在编译中出现“参数未使用”的警告信息，可以写一句pdata= pdata；）
ptos为指向任务堆栈栈顶的指针。任务堆栈用来保存局部变量，函数参数，返回地址以及中断时的CPU寄存器内容。任务堆栈的大小决定于任务的需要及预计的中断嵌套层数。计算堆栈的大小，需要知道任务的局部变量所占的空间，可能产生嵌套调用的函数，及中断嵌套所需空间。如果初始化常量OS_STK_GROWTH设为1，堆栈被设为向低端增长（从内存高地址向低地址增长）。此时ptos应该指向任务堆栈空间的最高地址。反之，如果OS_STK_GROWTH设为0，堆栈将从低地址向高地址增长。
prio为任务的优先级。每个任务必须有一个唯一的优先级作为标识。数字越小，优先级越高。id是任务的标识，目前这个参数没有实际的用途，但保留在OSTaskCreateExt（）中供今后扩展，应用程序中可设置id与优先级相同。
pbos为指向堆栈底端的指针。如果初始化常量OS_STK_GROWTH设为1，堆栈被设为从内存高地址向低地址增长。此时pbos应该指向任务堆栈空间的最低地址。反之，如果OS_STK_GROWTH设为0，堆栈将从低地址向高地址增长。pbos应该指向堆栈空间的最高地址。参数pbos用于堆栈检测函数OSTaskStkChk（）。
stk_size 指定任务堆栈的大小。其单位由OS_STK定义：当OS_STK的类型定义为INT8U、INT16U、INT32U的时候， stk_size的单位为分别为字节（8位）、字（16位）和双字（32位）。pext是一个用户定义数据结构的指针，可作为TCB的扩展。例如，当任务切换时，用户定义的数据结构中可存放浮点寄存器的数值，任务运行时间，任务切入次数等等信息。opt存放与任务相关的操作信息。opt的低8位由μC/OS保留，用户不能使用。用户可以使用opt的高8位。每一种操作由opt中的一位或几位指定，当相应的位被置位时，表示选择某种操作。

当前的μC/OS版本支持下列操作：
OS_TASK_OPT_STK_CHK：决定是否进行任务堆栈检查。
OS_TASK_OPT_STK_CLR：决定是否清空堆栈。
OS_TASK_OPT_SAVE_FP：决定是否保存浮点寄存器的数值。此项操作仅当处理器有浮点硬件时有效。保存操作由硬件相关的代码完成。
其他操作请参考文件uCOS_II.H。
返回值
OSTaskCreateExt（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_EXIST：具有该优先级的任务已经存在。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
OS_NO_MORE_TCB：系统中没有OS_TCB可以分配给任务了。
注意/警告：任务堆栈必须声明为OS_STK类型。
在任务中必须进行μC/OS提供的下述过程之一：延时等待、任务挂起、等待事件发生（等待信号量，消息邮箱、消息队列），以使其他任务得到CPU。
用户程序中不能使用优先级0，1，2，3，以及OS_LOWEST_PRIO-3, OS_LOWEST_PRIO-2, OS_LOWEST_PRIO-1, OS_LOWEST_PRIO。这些优先级μC/OS系统保留，其余56个优先级提供给应用程序。

**INT8U OSTaskDel (INT8U prio);**
所属文件 OS_TASK.C 调用者任务 开关量 OS_TASK_DEL_EN
OSTaskDel（）函数删除一个指定优先级的任务。任务可以传递自己的优先级给OSTaskDel（），从而删除自身。如果任务不知道自己的优先级，还可以传递参数OS_PRIO_SELF。被删除的任务将回到休眠状态。任务被删除后可以用函数OSTaskCreate（）或OSTaskCreateExt（）重新建立。
参数
prio为指定要删除任务的优先级，也可以用参数OS_PRIO_SELF代替，此时，下一个优先级最高的就绪任务将开始运行。
返回值
OSTaskDel（）的返回值为下述之一： OS_NO_ERR：函数调用成功。
OS_TASK_DEL_IDLE：错误操作，试图删除空闲任务（Idle task）。
OS_TASK_DEL_ ERR：错误操作，指定要删除的任务不存在。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
OS_TASK_DEL_ISR：错误操作，试图在中断处理程序中删除任务。
注意/警告
OSTaskDel（）将判断用户是否试图删除μC/OS中的空闲任务（Idle task）。
在删除占用系统资源的任务时要小心，此时，为安全起见可以用另一个函数OSTaskDelReq（）。

**INT8U OSDelReq (INT8U prio);**
所属文件 OS_TASK.C 调用者任务 开关量 OS_TASK_DEL_EN
OSTaskDelReq（）函数请求一个任务删除自身。通常OSTaskDelReq（）用于删除一个占有系统资源的任务（例如任务建立了信号量）。对于此类任务，在删除任务之前应当先释放任务占用的系统资源。具体的做法是：在需要被删除的任务中调用OSTaskDelReq（）检测是否有其他任务的删除请求，如果有，则释放自身占用的资源，然后调用OSTaskDel（）删除自身。例如，假设任务5要删除任务10，而任务10占有系统资源，此时任务5不能直接调用OSTaskDel（10）删除任务10，而应该调用OSTaskDelReq（10）向任务10发送删除请求。在任务10中调用OSTaskDelReq（OS_PRIO_SELF），并检测返回值。如果返回OS_TASK_DEL_REQ，则表明有来自其他任务的删除请求，此时任务10应该先释放资源，然后调用OSTaskDel（OS_PRIO_SELF）删除自己。任务5可以循环调用OSTaskDelReq（10）并检测返回值，如果返回OS_TASK_NOT_EXIST，表明任务10已经成功删除。
参数
prio为要求删除任务的优先级。如果参数为OS_PRIO_SELF，则表示调用函数的任务正在查询是否有来自其他任务的删除请求。
返回值
OSTaskDelReq（）的返回值为下述之一：
OS_NO_ERR：删除请求已经被任务记录。
OS_TASK_NOT_EXIST：指定的任务不存。发送删除请求的任务可以等待此返回值，看删除是否成功。
OS_TASK_DEL_IDLE：错误操作，试图删除空闲任务（Idle task）。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO或没有设定OS_PRIO_SELF的值。
OS_TASK_DEL_REQ：当前任务收到来自其他任务的删除请求。
注意/警告
OSTaskDelReq（）将判断用户是否试图删除μC/OS中的空闲任务（Idle task）。

**INT8U OSTaskQuery ( INT8U prio, OS_TCB *pdata);**
所属文件 OS_TASK.C 调用者任务或中断 开关量无
OSTaskQuery（）用于获取任务信息，函数返回任务TCB的一个完整的拷贝。应用程序必须建立一个OS_TCB类型的数据结构容纳返回的数据。需要提醒用户的是，在对任务OS_TCB对象中的数据操作时要小心，尤其是数据项OSTCBNext和OSTCBPrev。它们分别指向TCB链表中的后一项和前一项。
参数
prio为指定要获取TCB内容的任务优先级，也可以指定参数OS_PRIO_SELF，获取调用任务的信息。
pdata指向一个OS_TCB类型的数据结构，容纳返回的任务TCB的一个拷贝。
返回值
OSTaskQuery（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_ERR：参数指定的任务非法。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
注意/警告
任务控制块（TCB）中所包含的数据成员取决于下述开关量在初始化时的设定（参见OS_CFG.H）
OS_TASK_CREATE_EN
OS_Q_EN
OS_MBOX_EN
OS_SEM_EN
OS_TASK_DEL_EN

**INT8U OSTaskResume ( INT8U prio);**
所属文件 OS_TASK.C 调用者任务 开关量 OS_TASK_SUSPEND_EN
OSTaskResume （）唤醒一个用OSTaskSuspend（）函数挂起的任务。OSTaskResume（）也是唯一能“解挂”挂起任务的函数。
参数
prio指定要唤醒任务的优先级。
返回值
OSTaskResume （）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_TASK_RESUME_PRIO：要唤醒的任务不存在。
OS_TASK_NOT_SUSPENDED：要唤醒的任务不在挂起状态。
OS_PRIO_INVALID：参数指定的优先级大于或等于OS_LOWEST_PRIO。

**INT8U OSTaskStkChk ( INT8U prio, OS_STK_DATA *pdata);**
所属文件 OS_TASK.C 调用者任务 开关量 OS_TASK_CREATE_EXT
OSTaskStkChk（）检查任务堆栈状态，计算指定任务堆栈中的未用空间和已用空间。使用OSTaskStkChk（）函数要求所检查的任务是被OSTaskCreateExt（）函数建立的，且opt参数中OS_TASK_OPT_STK_CHK操作项打开。
计算堆栈未用空间的方法是从堆栈底端向顶端逐个字节比较，检查堆栈中0的个数，直到一个非0的数值出现。这种方法的前提是堆栈建立时已经全部清零。要实现清零操作，需要在任务建立初始化堆栈时设置OS_TASK_OPT_STK_CLR为1。如果应用程序在初始化时已经将全部RAM清零，且不进行任务删除操作，也可以设置OS_TASK_OPT_STK_CLR为0，这将加快OSTaskCreateExt（）函数的执行速度。
参数
prio为指定要获取堆栈信息的任务优先级，也可以指定参数OS_PRIO_SELF，获取调用任务本身的信息。
pdata指向一个类型为OS_STK_DATA的数据结构，其中包含如下信息：
   INT32U OSFree;       
   INT32U OSUsed;       
返回值
OSTaskStkChk（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO，或未指定OS_PRIO_SELF。
OS_TASK_NOT_EXIST：指定的任务不存在。
OS_TASK_OPT_ERR：任务用OSTaskCreateExt（）函数建立的时候没有指定OS_TASK_OPT_STK_CHK操作，或者任务是用OSTaskCreate（）函数建立的。
注意/警告
函数的执行时间是由任务堆栈的大小决定的，事先不可预料。
在应用程序中可以把OS_STK_DATA结构中的数据项OSFree和OSUsed相加，可得到堆栈的大小。虽然原则上该函数可以在中断程序中调用，但由于该函数可能执行很长时间，所以实际中不提倡这种做法。

**INT8U OSTaskSuspend ( INT8U prio);**
所属文件 OS_TASK.C 调用者任务 开关量 OS_TASK_SUSPEND_EN
OSTaskSuspend（）无条件挂起一个任务。调用此函数的任务也可以传递参数OS_PRIO_SELF，挂起调用任务本身。当前任务挂起后，只有其他任务才能唤醒。任务挂起后，系统会重新进行任务调度，运行下一个优先级最高的就绪任务。唤醒挂起任务需要调用函数OSTaskResume（）。
任务的挂起是可以叠加到其他操作上的。例如，任务被挂起时正在进行延时操作，那么任务的唤醒就需要两个条件：延时的结束以及其他任务的唤醒操作。又如，任务被挂起时正在等待信号量，当任务从信号量的等待对列中清除后也不能立即运行，而必须等到唤醒操作后。
参数
prio为指定要获取挂起的任务优先级，也可以指定参数OS_PRIO_SELF，挂起任务本身。此时，下一个优先级最高的就绪任务将运行。
返回值
OSTaskSuspend（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_TASK_ SUSPEND_IDLE：试图挂起μC/OS-II中的空闲任务（Idle task）。此为非法操作。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO或没有设定OS_PRIO_SELF的值。
OS_TASK_ SUSPEND _PRIO：要挂起的任务不存在。
注意/警告
在程序中OSTaskSuspend（）和OSTaskResume（）应该成对使用。
用OSTaskSuspend（）挂起的任务只能用OSTaskResume（）唤醒。

**void OSTimeDly ( INT16U ticks);**
所属文件 OS_TIMC.C 调用者任务 开关量无
OSTimeDly（）将一个任务延时若干个时钟节拍。如果延时时间大于0，系统将立即进行任务调度。延时时间的长度可从0到65535个时钟节拍。延时时间0表示不进行延时，函数将立即返回调用者。延时的具体时间依赖于系统每秒钟有多少时钟节拍（由文件SO_CFG.H中的常量OS_TICKS_PER_SEC设定）。
参数
ticks为要延时的时钟节拍数。
注意/警告
注意到延时时间0表示不进行延时操作，而立即返回调用者。为了确保设定的延时时间，建议用户设定的时钟节拍数加1。例如，希望延时10个时钟节拍，可设定参数为11。


**void OSTimeDlyHMSM( INT8U hours，INT8U minutes，INT8U seconds，INT8U milli);**
所属文件 OS_TIMC.C 调用者任务 开关量无
OSTimeDlyHMSM（）将一个任务延时若干时间。延时的单位是小时、分、秒、毫秒。所以使用OSTimeDlyHMSM（）比OSTimeDly（）更方便。调用OSTimeDlyHMSM（）后，如果延时时间不为0，系统将立即进行任务调度。
参数
hours为延时小时数，范围从0-255。
minutes为延时分钟数，范围从0-59。
seconds为延时秒数，范围从0-59
milli为延时毫秒数，范围从0-999。需要说明的是，延时操作函数都是以时钟节拍为为单位的。实际的延时时间是时钟节拍的整数倍。例如系统每次时钟节拍间隔是10ms，如果设定延时为5ms，将不产生任何延时操作，而设定延时15ms，实际的延时是两个时钟节拍，也就是20ms。
返回值
OSTimeDlyHMSM（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_TIME_INVALID_MINUTES：参数错误，分钟数大于59。
OS_TIME_INVALID_SECONDS：参数错误，秒数大于59。
OS_TIME_INVALID_MILLI：参数错误，毫秒数大于999。
OS_TIME_ZERO_DLY：四个参数全为0。
注意/警告
OSTimeDlyHMSM（0，0，0，0）表示不进行延时操作，而立即返回调用者。另外，如果延时总时间超过65535个时钟节拍，将不能用OSTimeDlyResume（）函数终止延时并唤醒任务。

**void OSTimeDlyResume( INT8U prio);**
所属文件 OS_TIMC.C 调用者任务 开关量无
OSTimeDlyResume（）唤醒一个用OSTimeDly（）或OSTimeDlyHMSM（）函数延时的任务。
参数
prio为指定要唤醒任务的优先级。
返回值
OSTimeDlyResume（）的返回值为下述之一：
OS_NO_ERR：函数调用成功。
OS_PRIO_INVALID：参数指定的优先级大于OS_LOWEST_PRIO。
OS_TIME_NOT_DLY：要唤醒的任务不在延时状态。
OS_TASK_NOT_EXIST：指定的任务不存在。
注意/警告
用户不应该用OSTimeDlyResume（）去唤醒一个设置了等待超时操作，并且正在等待事件发生的任务。操作的结果是使该任务结束等待，除非的确希望这么做。
OSTimeDlyResume（）函数不能唤醒一个用OSTimeDlyHMSM（）延时，且延时时间总计超过65535个时钟节拍的任务。例如，如果系统时钟为100Hz，OSTimeDlyResume（）不能唤醒延时OSTimeDlyHMSM（0，10，55，350）或更长时间的任务。
（OSTimeDlyHMSM（0，10，55，350）共延时     [ 10 minutes *60 + （55+0.35）seconds ] *100 =65,535次时钟节拍）