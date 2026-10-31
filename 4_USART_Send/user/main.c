#include "stm32f10x.h"

void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size);

int main(void)
{
	

	GPIO_InitTypeDef GPIO_InitStruct;
/*
	//PA9 tx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	//PA10 rx
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	
*/

   //重映射
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);

	//PB6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	//PB7
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

	USART_InitTypeDef USART_InitStruct;

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;

	USART_Init(USART1,&USART_InitStruct);

	USART_Cmd(USART1,ENABLE);//闭合总开关

	uint8_t bytesToSend[] = {1,2,3,4,5};

	My_USART_SendBytes(USART1,bytesToSend,5);

	while(1)
	{	
	}
}

void My_USART_SendBytes(USART_TypeDef *USARTx,uint8_t *pData,uint16_t Size)
{
	for(uint32_t i=0; i<Size ; i++){

		// #1. 等待发送数据寄存器为空
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
	
	// #2. 写入发送寄存器当中
	USART_SendData(USART1,pData[i]);
	}
	
	//等待发送完成
	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);

	
}
