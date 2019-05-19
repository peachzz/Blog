/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/main.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <includes.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/
static   OS_STK      AppTaskStartStk[APP_TASK_START_STK_SIZE];

static   OS_STK      AppTaskLed1Stk[APP_TASK_LED1_STK_SIZE];
static   OS_STK      AppTaskLed2Stk[APP_TASK_LED2_STK_SIZE];
static   OS_STK      AppTaskLed3Stk[APP_TASK_LED3_STK_SIZE];



/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static  void    AppTaskCreate  (void);
static	void	AppEventCreate (void);

static  void    AppTaskStart   (void *p_arg);

static  void    AppTaskLed1   (void *p_arg);
static  void    AppTaskLed2   (void *p_arg);
static  void    AppTaskLed3   (void *p_arg);

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
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
    //    while (1)
    //    {
    //    }
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  AppTaskStart (void *p_arg)
{

    (void)p_arg;

    BSP_Init();                                                 /* Init BSP fncts.                                          */

    CPU_Init();                                                 /* Init CPU name & int. dis. time measuring fncts.          */

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



    //    while (DEF_TRUE)                                            /* Task body, always written as an infinite loop.           */
    //    {
    //        OSTimeDlyHMSM(0, 0, 0, 100);
    //    }
}

/*
*********************************************************************************************************
*                                      CREATE APPLICATION TASKS
*
* Description:  This function creates the application tasks.
*
* Arguments  :  none
*
* Returns    :  none
*********************************************************************************************************
*/
static  void    AppTaskLed1(void *p_arg)
{

    p_arg = p_arg;

    while(DEF_TRUE)
    {
        LED1_TOGGLE;
        OSTimeDlyHMSM(0, 0, 1, 0);
    }
}

static  void AppTaskLed2(void *p_arg)
{

    p_arg = p_arg;

    while(DEF_TRUE)
    {
        LED2_TOGGLE;
        OSTimeDlyHMSM(0, 0, 5, 0);
    }
}

static  void    AppTaskLed3(void *p_arg)
{

    p_arg = p_arg;
    uint16_t i = 0;

    GUI_SetBkColor(GUI_RED);
    GUI_Clear();

    GUI_SetColor(GUI_BLUE);
    GUI_SetFont(GUI_FONT_20B_ASCII);
    GUI_DispStringAt("YS-F1Pro board STemWin test!", 10, 10);
    GUI_Delay(10);
    LCD_BK_ON();
    while(DEF_TRUE)
    {
        GUI_DispDecAt(i++, 100, 40, 4);
        if(i > 9999) i = 0;
        if(i % 100 == 0)
        {
            printf("AppTaskLed3 is run %d\r\n", i);
        }
        OSTimeDlyHMSM(0, 0, 0, 50);
    }
}

static  void  AppTaskCreate (void)
{
    OSTaskCreateExt(AppTaskLed1,                               /* Create the Led1 task                                    */
                    (void *)0,
                    (OS_STK *)&AppTaskLed1Stk[APP_TASK_LED1_STK_SIZE - 1],
                    APP_TASK_LED1_PRIO,
                    APP_TASK_LED1_PRIO,
                    (OS_STK *)&AppTaskLed1Stk[0],
                    APP_TASK_LED1_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(AppTaskLed2,                               /* Create the Led2 task                                    */
                    (void *)0,
                    (OS_STK *)&AppTaskLed2Stk[APP_TASK_LED2_STK_SIZE - 1],
                    APP_TASK_LED2_PRIO,
                    APP_TASK_LED2_PRIO,
                    (OS_STK *)&AppTaskLed2Stk[0],
                    APP_TASK_LED2_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    OSTaskCreateExt(AppTaskLed3,                               /* Create the Led2 task                                    */
                    (void *)0,
                    (OS_STK *)&AppTaskLed3Stk[APP_TASK_LED3_STK_SIZE - 1],
                    APP_TASK_LED3_PRIO,
                    APP_TASK_LED3_PRIO,
                    (OS_STK *)&AppTaskLed3Stk[0],
                    APP_TASK_LED3_STK_SIZE,
                    (void *)0,
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
}



/*
*********************************************************************************************************
*                                          AppEventCreate()
*
* Description : Create application kernel objects tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static void AppEventCreate(void)
{
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* User can add his own implementation to report the file name and line number,
       ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while (1)
    {
    }
}
#endif

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
