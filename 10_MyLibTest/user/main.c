#include "stm32f10x.h"
#include "si2c.h"

SI2C_TypeDef si2c;

void My_OnBoardLED_Init(void);

int main(void)
{
	si2c.SCL_GPIOx = GPIOB;
	si2c.SCL_GPIO_Pin = GPIO_Pin_6;
	si2c.SDA_GPIOx = GPIOB;
	si2c.SDA_GPIO_Pin = GPIO_Pin_7;
	My_SI2C_Init(&si2c);

	My_OnBoardLED_Init();

	uint8_t commands[] = {0x00,0x8d,0x14,0xaf,0xa5};

	My_SI2C_SendBytes(&si2c,0x78,commands,5);

	uint8_t rcvd;
	My_SI2C_ReceiveBytes(&si2c,0x78,&rcvd,1);
	

	if((rcvd & (0x01<<6)) == 0)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
	}
	else
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	}


	while(1)
	{	
	}
}



void My_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);

	GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
}

