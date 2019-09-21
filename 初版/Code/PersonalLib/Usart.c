#include "Usart.h"
#include "Task.h"
#include "Timer.h"
#include "DEBUG.h"
#include "LowPower.h"

uint8_t ReceiveBufferTemp[3][RX_BUFFER_SIZE];
uint8_t ReceiveBuffer[3][RX_BUFFER_SIZE_X_4];
uint16_t ReceiveNum[3]={0},ReadStart[3]={0};

USART_TypeDef* USART123[3]={USART1,USART2,USART3};

void UsartInit(void)
{
//	Usart1Init();
//	Usart2Init();
//	BC26UsartInit();
}
void UsartSentData(USART_TypeDef* USARTx,uint8_t Data)
{
	USART_SendData(USARTx,Data);//检查是否调用了USART初始化函数
	while (USART_GetFlagStatus(USARTx,USART_FLAG_TXE) == RESET);
}
void UsartSentDatas(USART_TypeDef* USARTx,char *Data)	
{
	uint16_t i;
	for(i=0;Data[i]!='\0';i++)
		UsartSentData(USARTx,Data[i]);
}
