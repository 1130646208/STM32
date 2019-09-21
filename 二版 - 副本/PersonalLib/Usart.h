#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"

void Usart1Init(void);
void UsartSentData(USART_TypeDef* USARTx,uint8_t Data);
void UsartSentDatas(USART_TypeDef* USARTx,char *Data);



#endif

