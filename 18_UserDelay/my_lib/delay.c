/**
  ******************************************************************************
  * @file    delay.c
  * @author  铁头山羊
  * @version V 1.0.0
  * @date    2022年8月30日
  * @brief   延迟函数源文件
  ******************************************************************************
  */

#include "delay.h"

__IO uint64_t ulTicks;

static uint8_t delay_initialized_flag = 0;


//
// @简介：初始化延迟函数
// @返回值：无
//
void Delay_Init(void)
{
	if(!delay_initialized_flag)
	{
		delay_initialized_flag = 1;
		
		RCC_ClocksTypeDef clockinfo = {0};
		uint32_t tmp;
		
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE; // 禁止SYSTICK

		ulTicks = 0;

		RCC_GetClocksFreq(&clockinfo);

		SysTick->CTRL |= SysTick_CTRL_TICKINT; // 开启中断

		// 设置中断优先级为0
		SCB->SHP[7] = 0;

		// 设置自动重装值以保证1ms的时钟
		tmp =  clockinfo.HCLK_Frequency / 1000;
		if(tmp > 0x00ffffff)
		{
			tmp = tmp / 8;
			SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE; 
		}
		else
		{
			SysTick->CTRL |= SysTick_CTRL_CLKSOURCE; 
		}
		SysTick->LOAD = tmp - 1;

		SysTick->CTRL |= SysTick_CTRL_ENABLE; 
	}
}


// @简介：毫秒级延迟
// @参数：Delay - 延迟时长，以毫秒为单位(千分之一秒)
// @返回值：无
// @注意：不允许在中断响应函数中调用此方法
void Delay(uint32_t Delay)
{
	Delay_Init();
	
	uint64_t expiredTime = ulTicks + Delay;

	while(ulTicks <  expiredTime){}
}

// @简介：获取当前时间，以毫秒（千分之一秒）为单位
// @参数：无
// @返回值：当前时间，单位为毫秒（千分之一秒）
uint64_t GetTick(void)
{
	Delay_Init();
	
	return ulTicks;
}

