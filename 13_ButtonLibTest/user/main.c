#include "stm32f10x.h"
#include "usart.h"
#include "button.h"

Button_TypeDef button1;
uint32_t cnt = 0;

void App_USART1_Init(void);
void App_Button_Init(void);
void button_clicked_cb(uint8_t clicks);
void button_long_pressed_cb(uint8_t ticks); 


int main(void)
{
	App_USART1_Init();
	App_Button_Init();

	while(1)
	{	
		My_Button_Proc(&button1);
	}
}

void App_USART1_Init(void)
{
	// #1.初始化io引脚 PA9 Tx PA10 Rx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	// #2.初始化USART1
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	USART_InitTypeDef USART_InitStruct;
	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_Init(USART1,&USART_InitStruct);

	USART_Cmd(USART1,ENABLE);
}

void App_Button_Init(void)
{
	Button_InitTypeDef Button_InitStruct;

	Button_InitStruct.GPIOx = GPIOA;
	Button_InitStruct.GPIO_Pin = GPIO_Pin_0;
	Button_InitStruct.ClickInterval = 0;
	Button_InitStruct.LongPressTime = 0;
	Button_InitStruct.LongPressTickInterval = 0;
	Button_InitStruct.button_clicked_cb = button_clicked_cb;
	Button_InitStruct.button_pressed_cb = 0;
	Button_InitStruct.button_released_cb = 0;
	Button_InitStruct.button_long_pressed_cb = button_long_pressed_cb;

	My_Button_Init(&button1,&Button_InitStruct);

}

void button_clicked_cb(uint8_t clicks)
{
	if(clicks == 1 )
	{
		cnt++;
		My_USART_Printf(USART1,"%d\n",cnt);
	}
	else if(clicks == 2)
	{
		cnt=0;
		My_USART_Printf(USART1,"%d\n",cnt);
	}
}

void button_long_pressed_cb(uint8_t ticks)
{
	cnt++;
	My_USART_Printf(USART1,"%d\n",cnt);
}
