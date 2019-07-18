#include "bsp_SysTick.h"
uint32_t TimingDelay;
/**
 * @brief 启动系统滴答定时器 SysTick
 * @param  无
 * @retval 无
 */
void SysTick_Init(void)
{
    /* SystemFrequency / 1000 1ms 中断一次
     * SystemFrequency / 100000 10us 中断一次
     * SystemFrequency / 1000000 1us 中断一次
     */
    if(SysTick_Config(SystemCoreClock / 100000))
    {
        while (1);
    }
}

/**
 * @brief us 延时程序,10us 为一个单位
 * @param
 * @arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 10us = 10us
 * @retval 无
 */
void Delay_us(__IO u32 nTime)
{
    TimingDelay = nTime;

    while (TimingDelay != 0);
}

/**
 * @brief 获取节拍程序
 * @param 无
 * @retval 无
 * @attention 在 SysTick 中断函数 SysTick_Handler()调用
 */
void TimingDelay_Decrement(void)
{
    if (TimingDelay != 0x00)
    {
        TimingDelay--;
    }
}
