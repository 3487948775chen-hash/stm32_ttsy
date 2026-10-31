#include "stm32f10x.h"
#include "delay.h"
int main(void)
{
	//#1.开启GPIOC的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	//#2.初始化IO引脚，PC13 通用输出开漏模式 2MHz
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	GPIO_Init(GPIOC,&GPIO_InitStruct);

	//GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//写1

	//GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);//写0
	while(1)
	{	
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);//亮

		Delay(100);//延时100ms

		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);//灭

		Delay(100);//延时100ms
	}
}
