#ifndef __BSP_SYSTICK_H__
#define __BSP_SYSTICK_H__

#include "stm32f4xx_rcc.h"

#define Delay_Ms(x) Delay_us(x*100)

void SysTick_Init(void);
void TimingDelay_Decrement(void);
void Delay_us(__IO u32 nTime);

extern uint32_t TimingDelay;

#endif
