#ifndef __RS485_USARTx_H__
#define	__RS485_USARTx_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>
#include <stdio.h>

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
// 485  使能引脚GPIO RE/DE宏定义
#define RS485_REDE_GPIO_ClockCmd               RCC_APB2PeriphClockCmd
#define RS485_REDE_GPIO_CLK                    RCC_APB2Periph_GPIOB     
#define RS485_REDE_PORT                        GPIOB   
#define RS485_REDE_PIN                         GPIO_Pin_2

#define RS485_USARTx                           USART3
#define RS485_USARTx_BAUDRATE                  115200

#define RS485_USARTx_ClockCmd                  RCC_APB1PeriphClockCmd
#define RS485_USARTx_CLK                       RCC_APB1Periph_USART3

#define RS485_USARTx_GPIO_ClockCmd             RCC_APB2PeriphClockCmd
#define RS485_USARTx_TX_PORT                   GPIOB   
#define RS485_USARTx_TX_PIN                    GPIO_Pin_10
#define RS485_USARTx_TX_CLK                    RCC_APB2Periph_GPIOB 

#define RS485_USARTx_RX_PORT                   GPIOB 
#define RS485_USARTx_RX_PIN                    GPIO_Pin_11
#define RS485_USARTx_RX_CLK                    RCC_APB2Periph_GPIOB

/* 扩展变量 ------------------------------------------------------------------*/
/* 函数声明 ------------------------------------------------------------------*/
void RS485_USARTx_Config(void);

#endif /* __RS485_USARTx_H__ */

/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
