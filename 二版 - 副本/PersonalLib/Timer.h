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

typedef struct TimerStruct{
	CircularQueue TimerQueue;
	uint32_t LastCNT;
	uint32_t Counter;
	uint32_t MaxCount;
	uint32_t LoopTimes;
	uint32_t MAXLoopTimes;
	uint8_t Using;
}Timer;

void TimeInit(void);
void TimerAdd(Timer* TimerAddr,uint32_t MaxCount,uint32_t MAXLoopTimes);
void TimerSub(Timer *TimerAddr);
uint8_t TimerLoopTimeOut(Timer *TimerAddr);

#endif
