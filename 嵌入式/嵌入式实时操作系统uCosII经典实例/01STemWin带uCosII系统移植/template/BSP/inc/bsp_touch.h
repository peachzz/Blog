#ifndef __BSP_XPT2046_H__
#define	__BSP_XPT2046_H__

#include "stm32f10x.h"
/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* 类型定义 ------------------------------------------------------------------*/
#pragma pack(4)     // 4字节对齐
typedef struct 
{	
  int32_t cal_flag;
  int32_t lcd_x[5];
	int32_t lcd_y[5];
  int32_t touch_x[5];
  int32_t touch_y[5];
	int32_t adjust[7];
}XPT2046_Calibration;
#pragma pack()

/* 宏定义 --------------------------------------------------------------------*/
/******************************* XPT2046 触摸屏中断引脚定义 ***************************/
#define XPT2046_EXTI_GPIO_CLK                        RCC_APB2Periph_GPIOE   
#define XPT2046_EXTI_GPIO_PORT                       GPIOE
#define XPT2046_EXTI_GPIO_PIN                        GPIO_Pin_2

#define XPT2046_EXTI_ActiveLevel                     0
#define XPT2046_EXTI_Read()                          GPIO_ReadInputDataBit(XPT2046_EXTI_GPIO_PORT,XPT2046_EXTI_GPIO_PIN )

/******************************* XPT2046 触摸屏模拟SPI引脚定义 ***************************/
#define XPT2046_SPI_GPIO_CLK                         RCC_APB2Periph_GPIOE

#define XPT2046_SPI_CS_PIN		                       GPIO_Pin_6
#define XPT2046_SPI_CS_PORT		                       GPIOE

#define	XPT2046_SPI_CLK_PIN	                         GPIO_Pin_3
#define XPT2046_SPI_CLK_PORT	                       GPIOE

#define	XPT2046_SPI_MOSI_PIN	                       GPIO_Pin_4
#define	XPT2046_SPI_MOSI_PORT	                       GPIOE

#define	XPT2046_SPI_MISO_PIN	                       GPIO_Pin_5
#define	XPT2046_SPI_MISO_PORT	                       GPIOE

#define XPT2046_CS_HIGT()                            GPIO_SetBits (XPT2046_SPI_CS_PORT,XPT2046_SPI_CS_PIN )    
#define XPT2046_CS_LOW()                             GPIO_ResetBits (XPT2046_SPI_CS_PORT,XPT2046_SPI_CS_PIN )  

#define XPT2046_CLK_HIGH()                           GPIO_SetBits (XPT2046_SPI_CLK_PORT,XPT2046_SPI_CLK_PIN )    
#define XPT2046_CLK_LOW()                            GPIO_ResetBits (XPT2046_SPI_CLK_PORT,XPT2046_SPI_CLK_PIN ) 

#define XPT2046_MOSI_HIGH()                          GPIO_SetBits (XPT2046_SPI_MOSI_PORT,XPT2046_SPI_MOSI_PIN ) 
#define XPT2046_MOSI_LOW()                           GPIO_ResetBits (XPT2046_SPI_MOSI_PORT,XPT2046_SPI_MOSI_PIN )

#define XPT2046_MISO()                               GPIO_ReadInputDataBit (XPT2046_SPI_MISO_PORT,XPT2046_SPI_MISO_PIN )

/******************************* XPT2046 触摸屏参数定义 ***************************/
#define	XPT2046_CHANNEL_X 	                          0x90 	          //驱动YN YP，测量X+	
#define	XPT2046_CHANNEL_Y 	                          0xd0	          //驱动XN XP，测量Y+

#define XPT2046_SPIFLASH_ADDR                         (0)

/* 扩展变量 ------------------------------------------------------------------*/
extern XPT2046_Calibration cal_value;

/* 函数声明 ------------------------------------------------------------------*/
void Touch_Init_GPIO(void);
uint8_t XPT2046_Touch_Calibrate(void);
void XPT2046_Get_TouchedPoint(uint16_t *pLCD_x,uint16_t *pLCD_y);
void Palette_draw_point(uint16_t x, uint16_t y,uint16_t usColor);
#endif /* __BSP_XPT2046_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
