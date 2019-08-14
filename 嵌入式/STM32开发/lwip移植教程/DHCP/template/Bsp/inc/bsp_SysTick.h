#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"

void SysTick_Init(void);
void Delay_ms(__IO u32 nTime);
//#define Delay_ms(x) Delay_us(100*x)	 //µ¥Î»ms
void TimingDelay_Decrement(void);
#endif /* __SYSTICK_H */
