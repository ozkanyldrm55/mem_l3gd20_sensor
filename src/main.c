/*https://engineeringvolkan.wordpress.com/tag/stm32f429-spi-kullanimi/*/

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <stdio.h>
#include <stdlib.h>

char str[50];
void gpio_init(void);
void usart_init(void);
void spi_yaz(uint8_t , uint8_t);
uint8_t spi_oku(uint8_t);
uint16_t x_data , y_data , z_data;
uint32_t i=32;


void delay(__IO uint32_t nCount)
{
	while(nCount--)
	{

	}
}


void USART_Puts(USART_TypeDef* USARTx , volatile char *s)
{
	while(*s)
	{
		while(!USARTx ->SR & 0x00000040);
		USART_SendData(USARTx , *s);
		*s++;
	}
}

void usart_init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct;
	USART_InitTypeDef 	USART_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);

	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

	USART_InitStruct.USART_BaudRate = 115200;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode = USART_Mode_Tx;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_InitStruct);
	USART_Cmd(USART1,ENABLE);
}

void gpio_init(void)
{
	SPI_InitTypeDef 	SPI_InitTypeDefStruct;
	GPIO_InitTypeDef	GPIO_InitTypeDefStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI5,ENABLE);

	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
	SPI_InitTypeDefStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeDefStruct.SPI_Mode = SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitTypeDefStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_InitTypeDefStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitTypeDefStruct.SPI_CPOL = SPI_CPOL_High;
	SPI_InitTypeDefStruct.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_Init(SPI5,&SPI_InitTypeDefStruct);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOC , ENABLE);

	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF,&GPIO_InitTypeDefStruct);

	GPIO_InitTypeDefStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitTypeDefStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitTypeDefStruct.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitTypeDefStruct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitTypeDefStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitTypeDefStruct);

	GPIO_PinAFConfig(GPIOF,GPIO_PinSource7,GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource8,GPIO_AF_SPI5);
	GPIO_PinAFConfig(GPIOF,GPIO_PinSource9,GPIO_AF_SPI5);
	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	SPI_Cmd(SPI5 , ENABLE);
}

void spi_yaz(uint8_t adress , uint8_t data)
{
	adress = 0x00 | adress;
	GPIO_ResetBits(GPIOC , GPIO_Pin_1);

	while(!SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI5,adress);
	while(!SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI5);
	while(!SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI5,adress);
	while(!SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_RXNE));
	SPI_I2S_ReceiveData(SPI5);

	GPIO_SetBits(GPIOC,GPIO_Pin_1);
}

uint8_t spi_oku(uint8_t adress)
{
	GPIO_ResetBits(GPIOC , GPIO_Pin_1);

	adress = 0X08 | adress;
	while(!SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI5,adress);
	while(!SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_RXNE));
	SPI_I2S_SendData(SPI5,adress);
	while(!SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_TXE));
	SPI_I2S_SendData(SPI5,adress);
	while(!SPI_I2S_GetFlagStatus(SPI5,SPI_I2S_FLAG_RXNE));
	SPI_I2S_SendData(SPI5,adress);

	GPIO_SetBits(GPIOC,GPIO_Pin_1);
	return SPI_I2S_ReceiveData(SPI5);
}

int main(void)
{

	gpio_init();
	usart_init();
	spi_yaz(0x20 , 0x0F);

  while (1)
  {
	  x_data = spi_oku(0x28);
	  y_data = spi_oku(0x2B);
	  z_data = spi_oku(0x2D);

	  sprintf(str , "x_data = %d \n""y_data = %d \n""z_data = %d \n",x_data,y_data,z_data);

	  USART_Puts(USART1,str);
	  delay(10000000L);

  }
}




void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size){
  /* TODO, implement your code here */
  return;
}

/*
 * Callback used by stm324xg_eval_audio_codec.c.
 * Refer to stm324xg_eval_audio_codec.h for more info.
 */
uint16_t EVAL_AUDIO_GetSampleCallBack(void){
  /* TODO, implement your code here */
  return -1;
}
