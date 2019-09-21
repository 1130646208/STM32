#include "stm32f10x_tim.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "CircularQueue.h"
#ifndef __TIMER_H__
#define __TIMER_H__

#define TIMER_NULL 0
#define TIMER_TIMEOUT 1
#define TIMER_NO_TIMEOUT 0
#define TIMER_USING 1
#define TIMER_NO_USING 0
#define TIMER_NO_SKIP_INTERRUPT 0
#define TIMER_SKIP_INTERRUPT 1

typedef struct TimerStruct{
	CircularQueue TimerQueue;
	uint16_t Counter;
	uint16_t MaxCount;
	uint16_t LoopTimes;
	uint16_t MAXLoopTimes;
	uint8_t Using;
}Timer;

void TimeInit(void);
void TimerAdd(Timer* TimerAddr,uint16_t MaxCount,uint16_t MAXLoopTimes);
void TimerSub(Timer *TimerAddr);
uint8_t TimerLoopTimeOut(Timer *TimerAddr);

#endif
