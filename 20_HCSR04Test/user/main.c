#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"

void App_USART1_Init(void);
void App_HCSR04_Init(void);

int main(void)
{
	App_USART1_Init();
	App_HCSR04_Init();

	while(1)
	{	
		// #1. 向CNT写0
		TIM_SetCounter(TIM1,0);

		// #2. 清除cc1和cc2标志位
		TIM_ClearFlag(TIM1,TIM_FLAG_CC1);
		TIM_ClearFlag(TIM1,TIM_FLAG_CC2);

		// #3. 开启定时器
		TIM_Cmd(TIM1,ENABLE);

		// #4. 向TRIG引脚发送10us的脉冲
		uint16_t trigger_start;
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);
		trigger_start = TIM_GetCounter(TIM1);

		while ((uint16_t)(TIM_GetCounter(TIM1) - trigger_start) < 10);

		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);

		// #5.等待测量完成
		while(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC1) == RESET);
		while(TIM_GetFlagStatus(TIM1,TIM_FLAG_CC2) == RESET);

		// #6. 关闭定时器
		TIM_Cmd(TIM1,DISABLE);

		uint16_t ccr1 = TIM_GetCapture1(TIM1);
		uint16_t ccr2 = TIM_GetCapture2(TIM1);
		

		float distance = (ccr2 - ccr1) * 1.0e-6 *340.0f / 2;

		My_USART_Printf(USART1,"distance = %.4f\r\n",distance);

		Delay(100);
	}
}

void App_USART1_Init(void)
{
	// #1. 初始化IO引脚
	GPIO_InitTypeDef GPIO_InitStruct;

	// PA9 AF_PP Tx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);

	// PA10 IPU Rx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;

	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
	// #2.初始化USART1
	// #2.1 开启USART1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	// #2.2 初始化UASRT1参数
	USART_InitTypeDef USART_InitStruct;

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1,&USART_InitStruct);

	// #2.3 闭合总开关
	USART_Cmd(USART1,ENABLE);
}

void App_HCSR04_Init(void)
{
	// #1.初始化时基单元
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);	

	// #2.初始化输入捕获
	// #2.1初始化IO引脚 PA8 IPD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	// #2.2初始化输入捕获通道1
	TIM_ICInitTypeDef TIM_IC_InitStruct;

	TIM_IC_InitStruct.TIM_Channel = TIM_Channel_1;
	TIM_IC_InitStruct.TIM_ICFilter = 0;
	TIM_IC_InitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_IC_InitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_IC_InitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;

	TIM_ICInit(TIM1,&TIM_IC_InitStruct);

	// #2.3初始化输入捕获通道2
	TIM_IC_InitStruct.TIM_Channel = TIM_Channel_2;
	TIM_IC_InitStruct.TIM_ICFilter = 0;
	TIM_IC_InitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_IC_InitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_IC_InitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;

	TIM_ICInit(TIM1,&TIM_IC_InitStruct);

	// #3.初始化triger引脚
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);

}
