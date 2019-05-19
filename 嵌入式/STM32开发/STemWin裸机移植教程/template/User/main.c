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
#include "stm32f10x.h"
#include <stdio.h>
#include "bsp_SysTick.h"
#include "bsp_lcd.h"
#include "bsp_debug_usart.h"
#include "stdlib.h"
#include "GUI.h"
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
    /* Add your application code here*/
//    uint16_t color;
    uint16_t i = 0;
    
    BSP_LCD_Init();
    DEBUG_USART_Init();
    
    SysTick_Init();
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
    GUI_Init();	
    GUI_SetBkColor(GUI_BLUE);
    GUI_Clear();

    /* ÏÔÊ¾²âÊÔ */
    GUI_SetColor(GUI_YELLOW);
    GUI_SetFont(GUI_FONT_20B_ASCII);
    GUI_DispStringAt("YS-F1Pro board STemWin test!", 10, 10);
    GUI_Delay(10);
    LCD_BK_ON();
    printf("System is running\r\n!");
    /* Infinite loop */
    while (1)
    {
        GUI_Delay(100);
        GUI_DispDecAt(i++, 100, 40, 4);
        if(i > 9999) i = 0;
    }
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
