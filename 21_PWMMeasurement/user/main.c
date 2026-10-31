#include "stm32f10x.h"
#include "usart.h"	
#include "delay.h"
#include "math.h"

void App_USART1_Init(void);
void App_TIM3_Init(void);
void App_TIM1_Init(void);

int main(void)
{
	App_USART1_Init();
	App_TIM3_Init();
	App_TIM1_Init();
	TIM_SetCompare1(TIM3,200);

	//My_USART_SendString(USART1,"你好世界");

	while(1)
	{	
		// #1.清除Trigger标志位
		TIM_ClearFlag(TIM1,TIM_FLAG_Trigger);

		// #2.等待Trigger标志位从0编程1
		while(TIM_GetFlagStatus(TIM1,TIM_FLAG_Trigger) == RESET);

		// #3.计算
		uint16_t ccr1 = TIM_GetCapture1(TIM1);
		uint16_t ccr2 = TIM_GetCapture2(TIM1);

		float period = ccr1 * 1.0e-6f * 1.0e3f;
		float duty = ((float)ccr2) / ccr1 *100.0f;

		My_USART_Printf(USART1,"周期=%.3fms,占空比=%.2f%%\r\n",period,duty);
		Delay(1000);


/*		float t =GetTick() * 1.0e-3f;
		float duty = 0.5*(sin(2*3.14*t) + 1);
		uint16_t ccr1 = duty *999;
*/
	}
}

void App_USART1_Init(void)
{
	// #1.初始化IO引脚 PA9 AF_PP
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	// #2.初始化USART1引脚
	// #2.1开启USART1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	// #2.2配置USART1的参数
	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1,&USART_InitStruct);

	// #2.3闭合总开关
	USART_Cmd(USART1,ENABLE);
}

void App_TIM3_Init(void)
{
	// #1.初始化时基单元
	// #1.1开启TIM3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	// #1.2配置时基单元的参数
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);

	// #1.3开启ARR的预加载
	TIM_ARRPreloadConfig(TIM3,ENABLE);

	// #1.4闭合时基单元的总开关
	TIM_Cmd(TIM3,ENABLE);

	// #2.初始化输出比较的通道1
	// #2.1 初始化PA6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	// #2.2配置OC1的参数
	TIM_OCInitTypeDef TIM_OCInitStruct;

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputState = ENABLE;
	TIM_OCInitStruct.TIM_Pulse = 0;

	TIM_OC1Init(TIM3,&TIM_OCInitStruct);

	// #2.3使能MOE
	TIM_CtrlPWMOutputs(TIM3,ENABLE);

	// #2.4 使能CCRx的预加载
	TIM_CCPreloadControl(TIM3,ENABLE);
}

void App_TIM1_Init(void)
{
	// #1.初始化时基单元
	// #1.1开启TIM1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	// #1.2配置时基单元的参数
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct = {0};

	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 65535;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);

	// #1.3开启ARR的预加载
	TIM_ARRPreloadConfig(TIM1,ENABLE);

	// #1.4闭合时基单元的总开关
	TIM_Cmd(TIM1,ENABLE);

	// 2.初始化输入捕获
	// #2.1初始化PA8引脚 IPD
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	// #2.2初始化输入捕获通道1
	TIM_ICInitTypeDef TIM_ICInitStruct;

	TIM_ICInitStruct.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInit(TIM1,&TIM_ICInitStruct);

	// #2.3初始化输入捕获通道2
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStruct.TIM_ICFilter = 0;
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Falling;
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_IndirectTI;
	TIM_ICInit(TIM1,&TIM_ICInitStruct);

	// #2.4初始化从模式控制器
	TIM_SelectInputTrigger(TIM1,TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM1,TIM_SlaveMode_Reset);	
}
