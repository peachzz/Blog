/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.32 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_X.C
Purpose     : This file provides emWin Interface with FreeRTOS
---------------------------END-OF-HEADER------------------------------
*/

/**
  ******************************************************************************
  * @attention
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/

/* uCosII include files */
#include "includes.h"

/*********************************************************************
*
* Global data
*/
static  OS_EVENT *DispSem;
//static  OS_EVENT *EventSem;
static OS_EVENT *EventMbox; 

static  OS_EVENT *KeySem;
static  int     KeyPressed;
static  char    KeyIsInited;

/*********************************************************************
*
* Timing:
* GUI_X_GetTime()
* GUI_X_Delay(int)

Some timing dependent routines require a GetTime
and delay function. Default time unit (tick), normally is
1 ms.
*/

int GUI_X_GetTime(void)
{
    return ((INT32U)OSTimeGet());
}

void GUI_X_Delay(int ms)
{
    INT32U ticks;
    ticks = (ms * 1000) / OS_TICKS_PER_SEC; 
    OSTimeDly(ticks);
}


/*********************************************************************
*
* GUI_X_ExecIdle
*
* Note:
* Called if WM is in idle state
*/

void GUI_X_ExecIdle(void)
{
    GUI_X_Delay(1);
}

/*********************************************************************
*
* Multitasking:
*
* GUI_X_InitOS()
* GUI_X_GetTaskId()
* GUI_X_Lock()
* GUI_X_Unlock()
*
* Note:
* The following routines are required only if emWin is used in a
* true multi task environment, which means you have more than one
* thread using the emWin API.
* In this case the
* #define GUI_OS 1
* needs to be in GUIConf.h
*/

/* Init OS */
void GUI_X_InitOS(void)
{
    /* 用于资源共享 cnt = 1*/
    DispSem =  OSSemCreate((INT16U)1);
//    /* 用于事件触发 cnt = 0*/
//    EventSem = OSSemCreate((INT16U)0);
    EventMbox = OSMboxCreate((void*)0); //创建消息邮箱
}



void GUI_X_Lock(void)
{
    INT8U err;
    OSSemPend((OS_EVENT *)DispSem,
              (INT32U )0,
              (INT8U *)&err);
}

void GUI_X_Unlock(void)
{
    OSSemPost((OS_EVENT *)DispSem);
}

/* Get Task handle */
U32 GUI_X_GetTaskId(void)
{
    return ((INT8U)(OSTCBCur->OSTCBPrio));
}


void GUI_X_WaitEvent (void)
{
    INT8U err;
    OSMboxPend((OS_EVENT *)EventMbox,
              (INT32U )0,
              (INT8U *)&err);
}


void GUI_X_SignalEvent (void)
{
    OSMboxPost((OS_EVENT *) EventMbox,(void *)1);
}

static  void  CheckInit (void)
{
    if (KeyIsInited == DEF_FALSE)
    {
        KeyIsInited = DEF_TRUE;
        GUI_X_Init();
    }
}

/*********************************************************************
*
* GUI_X_Init()
*
* Note:
* GUI_X_Init() is called from GUI_Init is a possibility to init
* some hardware which needs to be up and running before the GUI.
* If not required, leave this routine blank.
*/

void GUI_X_Init(void)
{

    KeySem = OSSemCreate((INT16U)0);
}

int  GUI_X_GetKey (void)
{
    int r;

    r          = KeyPressed;
    CheckInit();
    KeyPressed = 0;
    return (r);
}

int  GUI_X_WaitKey (void)
{
    int    r;
    INT8U err;


    CheckInit();
    if (KeyPressed == 0)
    {
        OSSemPend((OS_EVENT *)KeySem,
                  (INT32U )0,
                  (INT8U *)&err);
    }
    r          = KeyPressed;
    KeyPressed = 0;
    return (r);
}

void  GUI_X_StoreKey (int k)
{
    KeyPressed = k;
    OSSemPost((OS_EVENT *)KeySem);
}
/*********************************************************************
*
* Logging: OS dependent

Note:
Logging is used in higher debug levels only. The typical target
build does not use logging and does therefor not require any of
the logging routines below. For a release build without logging
the routines below may be eliminated to save some space.
(If the linker is not function aware and eliminates unreferenced
functions automatically)

*/

void GUI_X_Log     (const char *s)
{
    GUI_USE_PARA(s);
}
void GUI_X_Warn    (const char *s)
{
    GUI_USE_PARA(s);
}
void GUI_X_ErrorOut(const char *s)
{
    GUI_USE_PARA(s);
}


/*************************** End of file ****************************/
