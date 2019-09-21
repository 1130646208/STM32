#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include "Task.h"

#define RX_BUFFER_SIZE 128
#define RX_BUFFER_SIZE_X_4 4*RX_BUFFER_SIZE

void Usart1Init(void);
void UsartSentData(USART_TypeDef* USARTx,uint8_t Data);
void UsartSentDatas(USART_TypeDef* USARTx,char *Data);

#endif

