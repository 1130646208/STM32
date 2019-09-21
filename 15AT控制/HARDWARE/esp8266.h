#ifndef __ESP8266_H
#define __ESP8266_H

#include "usart.h"
#include "stm32f10x_usart.h"
#include "timer.h"
#include "oled.h"
#include "test.h"
#include "stdbool.h"

void ESP8266_Init(uint32_t br);
uint8_t  ESP8266_CMD_Handle(uint8_t cmdx,uint8_t timeout);
char * ESP8266_Listen(void);
bool ESP8266_Send_Data(char *data,uint8_t length);

#endif
