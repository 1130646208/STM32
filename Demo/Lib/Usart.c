#include "Usart.h"
#include "BC26RX.h"

void Usart1Init(void)
{
	USART_InitTypeDef UsartInit;
	NVIC_InitTypeDef NVICUsart1;
	GPIO_InitTypeDef GPIOAInit;
	USART_DeInit(USART1);		
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);	
	
	GPIOAInit.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIOAInit.GPIO_Pin=GPIO_Pin_9;
	GPIOAInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOAInit);
	
	GPIOAInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
	GPIOAInit.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIOAInit);
	
	UsartInit.USART_BaudRate=115200;		
	UsartInit.USART_WordLength=USART_WordLength_8b;		
	UsartInit.USART_StopBits=USART_StopBits_1;		
	UsartInit.USART_Parity=USART_Parity_No;		
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		
	USART_Init(USART1,&UsartInit);	
	
	NVICUsart1.NVIC_IRQChannel=USART1_IRQn;		
	NVICUsart1.NVIC_IRQChannelPreemptionPriority=1;	
	NVICUsart1.NVIC_IRQChannelSubPriority=0;
	NVICUsart1.NVIC_IRQChannelCmd=ENABLE;		
	NVIC_Init(&NVICUsart1);		
	USART_ClearFlag(USART1,USART_FLAG_TC);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		
	USART_Cmd(USART1,ENABLE);		
	
}
void Usart3Init(void)
{
	USART_InitTypeDef UsartInit;
	NVIC_InitTypeDef NVICUsart3;
	GPIO_InitTypeDef GPIOInit;
	USART_DeInit(USART3);		
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIOInit.GPIO_Pin=GPIO_Pin_10;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInit);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
	GPIOInit.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIOInit);
	
	UsartInit.USART_BaudRate=115200;		
	UsartInit.USART_WordLength=USART_WordLength_8b;		
	UsartInit.USART_StopBits=USART_StopBits_1;		
	UsartInit.USART_Parity=USART_Parity_No;		
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		
	USART_Init(USART3,&UsartInit);	
	
	NVICUsart3.NVIC_IRQChannel=USART3_IRQn;		
	NVICUsart3.NVIC_IRQChannelPreemptionPriority=0;	
	NVICUsart3.NVIC_IRQChannelSubPriority=0;
	NVICUsart3.NVIC_IRQChannelCmd=ENABLE;		
	NVIC_Init(&NVICUsart3);		
	USART_ClearFlag(USART3,USART_FLAG_TC);		
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		
	USART_Cmd(USART3,ENABLE);		
}
//void Usart2Init(void)
//{
//	USART_InitTypeDef UsartInit;
//	NVIC_InitTypeDef NVICUsart2;
//	GPIO_InitTypeDef GPIOInit;
//	USART_DeInit(USART2);		
//	
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//	
//	GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;	
//	GPIOInit.GPIO_Pin=GPIO_Pin_2;
//	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA,&GPIOInit);
//	
//	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
//	GPIOInit.GPIO_Pin=GPIO_Pin_3;
//	GPIO_Init(GPIOA,&GPIOInit);
//	
//	UsartInit.USART_BaudRate=38400;		
//	UsartInit.USART_WordLength=USART_WordLength_8b;		
//	UsartInit.USART_StopBits=USART_StopBits_1;		
//	UsartInit.USART_Parity=USART_Parity_No;		
//	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		
//	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		
//	USART_Init(USART2,&UsartInit);	
//	
//	NVICUsart2.NVIC_IRQChannel=USART2_IRQn;		
//	NVICUsart2.NVIC_IRQChannelPreemptionPriority=0;	
//	NVICUsart2.NVIC_IRQChannelSubPriority=1;
//	NVICUsart2.NVIC_IRQChannelCmd=ENABLE;		
//	NVIC_Init(&NVICUsart2);		
//	USART_ClearFlag(USART2,USART_FLAG_TC);		
//	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);		
//	USART_Cmd(USART2,ENABLE);		
//}
void UsartSentData(USART_TypeDef* Usart,uint8_t Data)		
{
	USART_SendData(Usart,Data);
	while (USART_GetFlagStatus(Usart, USART_FLAG_TXE) == RESET);
}
void UsartSentDatas(USART_TypeDef* Usart,uint8_t *Data)		
{
	uint8_t i;
	for(i=0;Data[i]!='\0';i++)
		UsartSentData(Usart,Data[i]);
}
void USART1_IRQHandler()		
{
	static uint8_t RXData;
	if (USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		RXData=USART_ReceiveData(USART1);
	}
}
//void USART2_IRQHandler()		
//{
//	static uint8_t RXData;
//	if (USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
//		RXData=USART_ReceiveData(USART2);
//		BTDataHandle(RXData);
//	}
//}
void USART3_IRQHandler()		
{
	static uint8_t RXData;
	if (USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);		
		RXData=USART_ReceiveData(USART3);
		BC26DataHandle(RXData);
	}
}
