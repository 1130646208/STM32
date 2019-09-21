#ifndef __TIMER3PWM_H
#define __TIMER3PWM_H


#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

void TIM3_PWM_Init(uint16_t arr,uint16_t psc);

#endif
