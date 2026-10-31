#include "stm32f10x.h"
#include "button.h"

Button_TypeDef button;

void App_SPI1_Init(void);
void App_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx, const uint8_t *pDataTx, uint8_t *pDataRx, uint16_t Size );
void App_W25Q64_SaveByte(uint8_t Byte);
uint8_t App_W25Q64_LoadByte(void);
void App_OnBoardLED_Init(void);
void App_Button_Init(void);
void button_clicked_cb(uint8_t clicks);

int main(void)
{
	App_SPI1_Init();
	App_OnBoardLED_Init();
	App_Button_Init();

	uint8_t byte = App_W25Q64_LoadByte();

	if(byte == 0)
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
	}
	else
	{
		GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
	}


	while(1)
	{	
		My_Button_Proc(&button);
	}
}

void button_clicked_cb(uint8_t clicks)
{
	if(clicks == 1)
	{
		if(GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13) == Bit_SET)
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_RESET);
			App_W25Q64_SaveByte(0x01);
		}
		else
		{
			GPIO_WriteBit(GPIOC,GPIO_Pin_13,Bit_SET);
			App_W25Q64_SaveByte(0x00);
		}
	}
}

void App_Button_Init(void)
{
	Button_InitTypeDef Button_InitStruct = {0};

	Button_InitStruct.GPIOx = GPIOA;
	Button_InitStruct.GPIO_Pin = GPIO_Pin_0;
	Button_InitStruct.button_clicked_cb = button_clicked_cb;

	My_Button_Init(&button,&Button_InitStruct);
}

void App_OnBoardLED_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
}

void App_SPI1_Init(void)
{
	// #1. 初始化IO引脚

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1,ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;

	// PB3 SCK AF_PP 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	// PB4 MISO IPU 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	// PB5 MOSI AF_PP 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	// PA15 普通IO Out_pp 2MHz
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);

	// #2. 对SPI本身进行初始化
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

	SPI_InitTypeDef SPI_InitStruct;
	SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;

	SPI_Init(SPI1,&SPI_InitStruct);

	SPI_NSSInternalSoftwareConfig(SPI1,SPI_NSSInternalSoft_Set);
}

void App_SPI_MasterTransmitReceive(SPI_TypeDef *SPIx, const uint8_t *pDataTx, uint8_t *pDataRx, uint16_t Size )
{
	// #1. 闭合总开关
	SPI_Cmd(SPIx,ENABLE);

	// #2. 写入第一个字节
	SPI_I2S_SendData(SPIx,pDataTx[0]);

	// #3.
	for(uint16_t i=0; i<Size-1; i++)
	{
		//向TDR写数据
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_TXE) == RESET);

		SPI_I2S_SendData(SPIx,pDataTx[i+1]);

		//向RDR读数据
		while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);

		pDataRx[i] = SPI_I2S_ReceiveData(SPIx);
	}

	// #4. 读取最后一个字节
	while(SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);
	
	pDataRx[Size-1] = SPI_I2S_ReceiveData(SPIx);

	// #5.断开总开关
	SPI_Cmd(SPIx,DISABLE);
}

void App_W25Q64_SaveByte(uint8_t Byte)
{
	uint8_t buffer[10];

	buffer[0] = 0x06;

	// #1. 写使能
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中

	App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 1);

	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);//取消选中

	// #2. 扇区擦除

	buffer[0] = 0x20;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;

	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中

	App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 4);

	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);//取消选中

	// #3. 等待空闲
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中

		//写0x05

		buffer[0] = 0x05;
		App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 1);

		//读状态寄存器1
		buffer[0] = 0xff;
		App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 1);

		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);//取消选中

		if((buffer[0] & 0x01) == 0)break;
	}

	// #4. 写使能

	buffer[0] = 0x06;

	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中

	App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 1);

	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);//取消选中

	// #5. 页编程
	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中

	buffer[0] = 0x02;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;
	buffer[4] = Byte;

	App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 5);

	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);//取消选中

	// #6. 等待空闲
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中

		//写0x05

		buffer[0] = 0x05;
		App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 1);

		//读状态寄存器1
		buffer[0] = 0xff;
		App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 1);

		GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);//取消选中

		if((buffer[0] & 0x01) == 0)break;
	}

}

uint8_t App_W25Q64_LoadByte(void)
{
	uint8_t buffer[10];

	buffer[0] = 0x03;
	buffer[1] = 0x00;
	buffer[2] = 0x00;
	buffer[3] = 0x00;

	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_RESET);//选中

	App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 4);

	buffer[0] = 0xff;
	App_SPI_MasterTransmitReceive(SPI1, buffer, buffer, 1);

	GPIO_WriteBit(GPIOA,GPIO_Pin_15,Bit_SET);//取消选中

	return buffer[0];

}
