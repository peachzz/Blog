/****************************************************************************
* Copyright (C), 2011 奋斗嵌入式工作室 www.ourstm.net
*
* 本例程在 奋斗版STM32开发板V2,2.1,V3上调试通过
* QQ: 9191274, 旺旺：sun68, Email: sun68@163.com
* 淘宝店铺：ourstm.taobao.com
*
* 文件名: spi.c
* 内容简述:
*       本例程包含了SPI1 ENC28J60的底层驱动函数
*
* 文件历史:
* 版本号  日期       作者    说明
* v0.2    2011-8-17 sun68  创建该文件
*
*/
#include <STM32F10X_SPI.h>
#include <STM32F10X_GPIO.h>
#include <STM32F10X_RCC.h>
#include "spi.h"

unsigned char	SPI1_ReadWrite(unsigned char writedat);
void SPI1_SetSpeed(u8 SpeedSet);

//__lwip_dev lwipdev;
/****************************************************************************
* 名    称：void SPI1_Init(void)
* 功    能：SPI1初始化
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
void SPI1_Init(void)
{
    //	u32 temp;
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能SPI1时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

    /* 配置SPI1的SCK,MISO MOSI */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			  //复用功能
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 配置SPI1的ENC28J60片选 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_SetBits(GPIOA, GPIO_Pin_4);

    /* SPI1配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    /* 使能SPI1  */
    SPI_Cmd(SPI1, ENABLE);

    //	SPI1_SetSpeed(SPI_BaudRatePrescaler_8);	//SPI1 SCK频率为36M/4=4.5Mhz
    //初始化MAC地址
    //	temp=*(vu32*)(0x1FFFF7E8);	//获取STM32的唯一ID的前24位作为MAC地址后三字节
    //	lwipdev.mac[0]=2;
    //	lwipdev.mac[1]=0;
    //	lwipdev.mac[2]=0;
    //	lwipdev.mac[3]=(temp>>16)&0XFF;	//低三字节用STM32的唯一ID
    //	lwipdev.mac[4]=(temp>>8)&0XFFF;
    //	lwipdev.mac[5]=temp&0XFF;

}

/****************************************************************************
* 名    称：unsigned char	SPI1_ReadWrite(unsigned char writedat)
* 功    能：SPI1读写一字节程序
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/
unsigned char	SPI1_ReadWrite(unsigned char writedat)
{
    /* Loop while DR register in not emplty */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

    /* Send byte through the SPI1 peripheral */
    SPI_I2S_SendData(SPI1, writedat);

    /* Wait to receive a byte */
    while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);
}

/****************************************************************************
* 名    称：void EXTI_Inits(void)
* 功    能：ENC28J60外部中断配置
* 入口参数：无
* 出口参数：无
* 说    明：
* 调用方法：无
****************************************************************************/

void ENC28J60_EXTI_Inits(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_InitTypeDef GPIO_InitStructure;

    //初始化PA1管脚
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	         	 	//ENC28J60接收完成中断引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   	 		//内部上拉输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Enable the EXTI2 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;				 //外部中断1
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	     //抢占优先级 0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			 //子优先级0
#ifdef LWIP_INTERRUPT
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				 //使能
#endif
    NVIC_Init(&NVIC_InitStructure);


    //用于配置AFIO外部中断配置寄存器AFIO_EXTICR1，用于选择EXTI2外部中断的输入源是PE2。
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);     //外部中断配置AFIO--ETXI2

    EXTI_InitStructure.EXTI_Line = EXTI_Line1;					  //PE2 作为键盘的行线。检测状态
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;			  //中断模式
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;		  //下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)

void SPI1_SetSpeed(u8 SpeedSet)
{
    //SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
    //SPI_Init(SPI1, &SPI_InitStructure);
    //	SPI_Cmd(SPI1,ENABLE);
    SPI1->CR1 &= 0XFFC7;
    SPI1->CR1 |= SpeedSet;	//设置SPI1速度
    SPI1->CR1 |= 1 << 6; 		//SPI设备使能
}


//void GPIOA1_Init(void)
//{

//	GPIO_InitTypeDef GPIO_InitStructure;
//
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	         	 	//ENC28J60接收完成中断引脚
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;   	 		//内部上拉输入
//  GPIO_Init(GPIOA, &GPIO_InitStructure);
//}
