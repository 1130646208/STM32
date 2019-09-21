#ifndef __USART_H__
#define __USART_H__

#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"
#include "misc.h"

void Usart1Init(void);
void Usart3Init(void);
void Usart2Init(void);
void UsartSentData(USART_TypeDef* Usart,uint8_t Data);
void UsartSentDatas(USART_TypeDef* Usart,uint8_t *Data);


#endif