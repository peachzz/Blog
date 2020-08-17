/**
  ******************************************************************************
  * 文件名程: bsp_usartx_485.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 板载RS485底层驱动程序
  *           不管是作为主机或作为从机驱动程序是一样的
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F1Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp/485/bsp_usartx_485.h"
#include <stdarg.h>

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: USARTx GPIO 配置,工作模式配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void RS485_USARTx_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* config RS485_USARTx clock */
	RS485_USARTx_GPIO_ClockCmd(RS485_USARTx_TX_CLK|RS485_USARTx_RX_CLK, ENABLE);
	RS485_REDE_GPIO_ClockCmd(RS485_REDE_GPIO_CLK, ENABLE);
	RS485_USARTx_ClockCmd(RS485_USARTx_CLK, ENABLE);

	/* RS485_USARTx GPIO config */
   /* Configure RS485_USARTx Tx  as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = RS485_USARTx_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_USARTx_TX_PORT, &GPIO_InitStructure);
	    
  /* Configure RS485_USARTx Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = RS485_USARTx_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(RS485_USARTx_RX_PORT, &GPIO_InitStructure);
  
	/* RS485接收发送使能GPIO */
  GPIO_InitStructure.GPIO_Pin = RS485_REDE_PIN ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(RS485_REDE_PORT, &GPIO_InitStructure);
	  
	/* USART工作环境配置 */
  /* USART波特率:115200 */
	USART_InitStructure.USART_BaudRate = RS485_USARTx_BAUDRATE;
	/* USART字长(有效位)：8位 */
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	/* USART停止位：1位 */
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
	/* USART校验位：无 */
  USART_InitStructure.USART_Parity = USART_Parity_No ;
	/* USART硬件数据流控制(硬件信号控制传输停止)：无 */
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	/* USART工作模式使能：允许接收和发送 */
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	/* 初始化USART */
  USART_Init(RS485_USARTx, &USART_InitStructure); 
  /* 使能USART */
  USART_Cmd(RS485_USARTx, ENABLE);
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
