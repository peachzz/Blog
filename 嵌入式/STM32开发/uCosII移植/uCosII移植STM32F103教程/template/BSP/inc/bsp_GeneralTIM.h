#ifndef __GENERAL_TIM_H__
#define __GENERAL_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include <stm32f10x.h>

/* 类型定义 ------------------------------------------------------------------*/
/* 宏定义 --------------------------------------------------------------------*/
/********************通用定时器TIM参数定义，只限TIM2 & TIM3 & TIM4 & TIM5************/
#define GENERAL_TIMx                 TIM2
#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM2
#define GENERAL_TIM_IRQ              TIM2_IRQn
#define GENERAL_TIM_INT_FUN          TIM2_IRQHandler

//#define GENERAL_TIMx                 TIM3
//#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
//#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM3
//#define GENERAL_TIM_IRQ              TIM3_IRQn
//#define GENERAL_TIM_INT_FUN          TIM3_IRQHandler

//#define GENERAL_TIMx                 TIM4
//#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
//#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM4
//#define GENERAL_TIM_IRQ              TIM4_IRQn
//#define GENERAL_TIM_INT_FUN          TIM4_IRQHandler

//#define GENERAL_TIMx                 TIM5
//#define GENERAL_TIM_APBxClock_FUN    RCC_APB1PeriphClockCmd
//#define GENERAL_TIM_CLK              RCC_APB1Periph_TIM5
//#define GENERAL_TIM_IRQ              TIM5_IRQn
//#define GENERAL_TIM_INT_FUN          TIM5_IRQHandler

/* 扩展变量 ------------------------------------------------------------------*/
extern __IO uint16_t timer_count;
/* 函数声明 ------------------------------------------------------------------*/
void GENERAL_TIMx_Configuration(void);

#endif	/* __GENERAL_TIM_H__ */
/******************* (C) COPYRIGHT 2015-2020 硬石嵌入式开发团队 *****END OF FILE****/
