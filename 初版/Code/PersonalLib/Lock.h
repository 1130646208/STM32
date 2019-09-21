#ifndef __LOCK_H__
#define __LOCK_H__

#include "stm32f10x.h"

#define LOCK_RCC RCC_APB2Periph_GPIOA
#define LOCK_PORT GPIOA
#define LOCK_PIN_1 GPIO_Pin_4
#define LOCK_PIN_2 GPIO_Pin_5
#define LOCK_DOOR_CHECK_PIN GPIO_Pin_6
#define LOCK_CHECK_PIN GPIO_Pin_7

extern char ClientID[17];
extern char Status[17];
extern char HeartTimes[17];
extern char AuthCode[17];
extern char BTPassword[17];
extern uint8_t IDChanged;

#endif
