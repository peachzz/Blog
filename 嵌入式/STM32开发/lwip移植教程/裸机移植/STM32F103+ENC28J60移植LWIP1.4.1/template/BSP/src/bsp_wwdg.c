 /**
  ******************************************************************************
  * 文件名程: bsp_wwdg.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2015-10-04
  * 功    能: 窗口看门狗底层驱动程序
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
#include "bsp_wwdg.h"

/* 私有类型定义 --------------------------------------------------------------*/
/* 私有宏定义 ----------------------------------------------------------------*/
/* 私有变量 ------------------------------------------------------------------*/
/* 扩展变量 ------------------------------------------------------------------*/
/* 私有函数原形 --------------------------------------------------------------*/
/* 函数体 --------------------------------------------------------------------*/
/**
  * 函数功能: WWDG 中断优先级初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
static void WWDG_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  /* 选择中断优先级配置组为2个抢占式优先级和2个子优先级，可以参考misc.h文件了解相关设置 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 
	/* 使能WWDG所在的外部中断通道 */
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQn;
	/* 设置抢占式优先级为0 */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	/* 设置子优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	/* 使能外部中断通道 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * 函数功能: WWDG配置
  * 输入参数: tr ：递减计时器的值， 取值范围为：0x7f~0x40
  *           wr ：窗口值，取值范围为：0x7f~0x40
  *           prv：预分频器值，取值可以是
  *              参数 WWDG_Prescaler_1: WWDG counter clock = (PCLK1/4096)/1
  *              参数 WWDG_Prescaler_2: WWDG counter clock = (PCLK1/4096)/2
  *              参数 WWDG_Prescaler_4: WWDG counter clock = (PCLK1/4096)/4
  *              参数 WWDG_Prescaler_8: WWDG counter clock = (PCLK1/4096)/8
  * 返 回 值: 无
  * 说    明：无
  */
void WWDG_Config(uint8_t tr, uint8_t wr, uint32_t prv)
{	
	/* 开启 WWDG 时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);
	
	/* 设置递减计数器的值 */
	WWDG_SetCounter( tr );
	
	/* 设置预分频器的值 */
	WWDG_SetPrescaler( prv );
	
	/* 设置上窗口值 */
	WWDG_SetWindowValue( wr );
	
	/* 设置计数器的值，使能WWDG */
	WWDG_Enable(WWDG_CNT);	
	
	/* 清除提前唤醒中断标志位 */
	WWDG_ClearFlag();	
	/* 配置WWDG中断优先级 */
	WWDG_NVIC_Config();	
	/* 开WWDG 中断 */
	WWDG_EnableIT();
}

/**
  * 函数功能: 喂狗
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
void WWDG_Feed(void)
{
	/* 喂狗，刷新递减计数器的值，设置成最大WDG_CNT=0X7F */
	WWDG_SetCounter( WWDG_CNT );
}
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
