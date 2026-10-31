#include "stm32f10x.h"

volatile uint32_t currentTick = 0;

void App_Delay(uint32_t ms);
void APP_TIM3_TimeBaseInit(void);
void APP_OnBoardLEDInit(void);

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	APP_TIM3_TimeBaseInit();
	APP_OnBoardLEDInit();
	while(1)
	{	
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);

		App_Delay(500);

		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);

		App_Delay(500);
	}
}

void App_Delay(uint32_t ms)
{
	uint32_t expireTime = currentTick + ms;

	while(currentTick < expireTime);
}

void APP_TIM3_TimeBaseInit(void)
{
	// #1.开启定时器3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

	// #2.配置时基单元的参数
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInittStruct;

	TIM_TimeBaseInittStruct.TIM_Prescaler = 71;
	TIM_TimeBaseInittStruct.TIM_Period = 999;
	TIM_TimeBaseInittStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInittStruct.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInittStruct);

	// #3.闭合时基单元开关 
	TIM_Cmd(TIM3,ENABLE);

	// #4.使能Update中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	// #5.配置NVIC模块
	NVIC_InitTypeDef NVIC_InitStruct;

	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;

	NVIC_Init(&NVIC_InitStruct);

}

void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3,TIM_FLAG_Update) == SET)
	{
		TIM_ClearFlag(TIM3,TIM_FLAG_Update);

		currentTick++;
	}
}

void APP_OnBoardLEDInit(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
}
