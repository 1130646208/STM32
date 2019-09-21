#ifndef __TIMER_H_
#define __TIMER_H_
#include "stm32f10x_tim.h"
#include "led.h"

extern uint16_t INT_TIMES;

void TIMER6_Init(uint16_t arr,uint16_t psc);
void TIM6_SetTime(uint16_t arr);

#endif
