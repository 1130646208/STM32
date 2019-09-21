#include "Timer.h"
#include "Usart.h"
#include "stdio.h"
CircularQueue* StartTimer=CIRCULARQUEUE_NULL;
CircularQueue* TimerTemp=CIRCULARQUEUE_NULL;
uint8_t NeedReAdd=0;


void TimeInit(void)		
{	
	TIM_TimeBaseInitTypeDef TIM_init_stru;
	NVIC_InitTypeDef NVIC_init_stru;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	TIM_init_stru.TIM_Period=10000-1;
	TIM_init_stru.TIM_Prescaler=36000-1;
	TIM_init_stru.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_init_stru.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_init_stru);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_init_stru.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_init_stru.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_init_stru.NVIC_IRQChannelSubPriority=2;
	NVIC_init_stru.NVIC_IRQChannelCmd=ENABLE;
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	NVIC_Init(&NVIC_init_stru);
	
	TIM_Cmd(TIM2,DISABLE);
}
void TimerAdd(Timer* TimerAddr,uint32_t MaxCount,uint32_t MAXLoopTimes)		
{
	if(TimerAddr->Using==TIMER_USING)
		return ;
	TIM2->DIER &= (uint16_t)~TIM_IT_Update;		
	if(StartTimer==CIRCULARQUEUE_NULL){
		TIM_SetCounter(TIM2,0);
		TIM_Cmd(TIM2,ENABLE);
	}
	TimerAddr->TimerQueue.Data=TimerAddr;		
	TimerAddr->TimerQueue.StartAddr=&StartTimer;	
	TimerAddr->Counter=0;
	TimerAddr->LoopTimes=0;
	TimerAddr->LastCNT=TIM2->CNT;
	TimerAddr->MaxCount=MaxCount*2;
	TimerAddr->MAXLoopTimes=MAXLoopTimes;
	
	QueueAdd(&(TimerAddr->TimerQueue));
	TimerAddr->Using=TIMER_USING;
	TIM2->DIER |= TIM_IT_Update;
}
void TimerSub(Timer *TimerAddr)
{
	if(TimerAddr->Using==TIMER_NO_USING)
		return ;
	TIM2->DIER &= (uint16_t)~TIM_IT_Update;	
	TimerAddr->Using=TIMER_NO_USING;
	TimerAddr->Counter=0;
	TimerAddr->LastCNT=0;
	TimerAddr->LoopTimes=0;
	QueueSub(&(TimerAddr->TimerQueue));
	if(StartTimer==CIRCULARQUEUE_NULL){
		TIM_Cmd(TIM2,DISABLE);
		TIM_SetCounter(TIM2,0);
	}
	TIM2->DIER |= TIM_IT_Update;
}
uint8_t TimerLoopTimeOut(Timer *TimerAddr)
{
	uint16_t TimeCNT;
	TimeCNT=TIM2->CNT;
	TIM2->DIER &= (uint16_t)~TIM_IT_Update;
	if((TimerAddr->Using)==TIMER_NO_USING)
		return TIMER_NO_TIMEOUT;
	if((TimerAddr->Counter*10000+TimeCNT-TimerAddr->LastCNT)>=(TimerAddr->MaxCount)){
		TimerAddr->Counter=0;
		TimerAddr->LastCNT=TimeCNT;
		TimerAddr->LoopTimes++;
	}
	TIM2->DIER |= TIM_IT_Update;
	if(TimerAddr->LoopTimes>=TimerAddr->MAXLoopTimes){
		TimerAddr->LoopTimes=0;
		return TIMER_TIMEOUT;
	}
	else
		return TIMER_NO_TIMEOUT;
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET){
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
		if((StartTimer!=CIRCULARQUEUE_NULL)){
			TimerTemp=StartTimer;
			do{
				(((Timer*)(TimerTemp->Data))->Counter)++;
				TimerTemp=TimerTemp->NextOne;
			}while(TimerTemp!=StartTimer);
		}
	}
}
