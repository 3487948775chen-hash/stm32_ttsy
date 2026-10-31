#include "stm32f10x.h"
#include "delay.h"
#include "math.h"

void App_PWM_Init(void);

int main(void)
{
	App_PWM_Init();

	while(1)
	{	
		float t = GetTick() * 1.0e-3f;
		float duty = 0.5*(sin(2*3.14*t)+1);
		uint16_t ccr1 = duty * 1000;

		TIM_SetCompare1(TIM1,ccr1);

	}
}

void App_PWM_Init(void)
{
	// #1.初始化IO引脚 PA8 PB13
	//PA8
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//PB13

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOB,&GPIO_InitStruct);

	// #2. 配置时基单元
	// #2.1 开启TIM1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);

	// #2.2 配置时基单元的参数
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;

	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period = 999;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);

	// #2.3 开启ARR预加载
	TIM_ARRPreloadConfig(TIM1,ENABLE);

	// #2.4 闭合时基单元的开关
	TIM_Cmd(TIM1,ENABLE);

	// #3. 初始化输出比较
	// #3.1 初始化输出比较通道1的参数

	TIM_OCInitTypeDef TIM_OCInitStruct;

	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStruct.TIM_OCNPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;

	TIM_OC1Init(TIM1,&TIM_OCInitStruct);

	// #3.2 闭合MOE总开关
	TIM_CtrlPWMOutputs(TIM1,ENABLE);

	TIM_CCPreloadControl(TIM1,ENABLE);


}
