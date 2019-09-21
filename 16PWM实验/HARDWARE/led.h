#ifndef __LED_H
#define __LED_H
#include "sys.h"
#include "stm32f10x_gpio.h"
#define LED0 PBout(5)
#define LED1 PEout(5)
void LED_Init(void);
#endif
