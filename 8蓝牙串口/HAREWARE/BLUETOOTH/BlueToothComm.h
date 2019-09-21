#ifndef __BLUETOOTHCOMM_H__
#define __BLUETOOTHCOMM_H__

#include "stdint.h"
#include "Usart.h"

#define BTMAXBUFFER 64	
#define BTMAXROW 8

extern uint8_t SetAuth[7];
extern uint8_t AuthCode[7];

void BTDataHandle(uint8_t RXData);

#endif