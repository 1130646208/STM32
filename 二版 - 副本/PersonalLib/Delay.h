#ifndef __DELAY_H__
#define __DELAY_H__

#include "stm32f10x.h"
#include "core_cm3.h"

void SystickStops(void);
void DelayInit(void);
void DelayUs(uint32_t times);
void DelayMs(uint32_t times);

#endif
