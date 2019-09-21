#ifndef __PUBTIMER_H
#define __PUBTIMER_H

#include "stm32f10x_tim.h"
#include "stm32f10x_it.h"
#include "DEBUG.h"
#include "BC26Task.h"

void pub_timer_init(u16 arr,u16 psc);

void start_pub(void);
void stop_pub(void);

#endif

