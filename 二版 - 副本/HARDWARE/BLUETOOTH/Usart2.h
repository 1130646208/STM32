#ifndef __USART2_H__
#define __USART2_H__

#include "stm32f10x.h"
#include "usart.h"
#include "flash.h"
#include "DEBUG.h"

#define RX_BUFFER_SIZE 128
#define RX_BUFFER_SIZE_X_4 4*RX_BUFFER_SIZE

extern uint8_t ReceiveBufferTemp[3][RX_BUFFER_SIZE];
extern uint8_t ReceiveBuffer[3][RX_BUFFER_SIZE_X_4];
extern uint16_t ReceiveNum[3],ReadStart[3];

void Usart2Init(void);

#endif

