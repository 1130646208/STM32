#include "Timer.h"

CircularQueue* StartTimer=CIRCULARQUEUE_NULL;
CircularQueue* TimerTemp=CIRCULARQUEUE_NULL;

uint8_t SkipThisinterrupt=TIMER_NO_SKIP_INTERRUPT;

void TimeInit(void)		
{	
	TIM_TimeBaseInitTypeDef TIM_init_stru;
	NVIC_InitTypeDef NVIC_init_stru;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	TIM_init_stru.TIM_Period=1000-1;
	TIM_init_stru.TIM_Prescaler=72-1;
	TIM_init_stru.TIM_ClockDivision=TIM_CKD_DIV1;
	TIM_init_stru.TIM_CounterMode=TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM2,&TIM_init_stru);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_init_stru.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_init_stru.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_init_stru.NVIC_IRQChannelSubPriority=0;
	NVIC_init_stru.NVIC_IRQChannelCmd=ENABLE;
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
	NVIC_Init(&NVIC_init_stru);
	
	TIM_Cmd(TIM2,ENABLE);
}
void TimerAdd(Timer* TimerAddr,uint16_t MaxCount,uint16_t MAXLoopTimes)		
{
	SkipThisinterrupt=TIMER_SKIP_INTERRUPT;		
	TimerAddr->TimerQueue.Data=TimerAddr;		
	TimerAddr->TimerQueue.StartAddr=&StartTimer;	
	//if(StartTimer==CIRCULARQUEUE_NULL)
		//TIM_Cmd(TIM2,ENABLE);
	QueueAdd(&(TimerAddr->TimerQueue));//(CircularQueue* CircularQueueAddr)
	TimerAddr->Counter=0;
	TimerAddr->LoopTimes=0;
	TimerAddr->MaxCount=MaxCount;
	TimerAddr->MAXLoopTimes=MAXLoopTimes;
	TimerAddr->Using=TIMER_USING;
	SkipThisinterrupt=TIMER_NO_SKIP_INTERRUPT;
}
void TimerSub(Timer *TimerAddr)
{
	SkipThisinterrupt=TIMER_SKIP_INTERRUPT;
	TimerAddr->Using=TIMER_NO_USING;
	//if(TimerAddr->TimerQueue.NextOne==(&(TimerAddr->TimerQueue)))
		//TIM_Cmd(TIM2,DISABLE);
	QueueSub(&(TimerAddr->TimerQueue));
	TimerAddr->Counter=0;
	TimerAddr->LoopTimes=0;
	SkipThisinterrupt=TIMER_NO_SKIP_INTERRUPT;
}
uint8_t TimerLoopTimeOut(Timer *TimerAddr)
{
	if(TimerAddr->Counter>=TimerAddr->MaxCount){
		TimerAddr->Counter=0;//重装载
		TimerAddr->LoopTimes++;//完成了一次计时
        
	}
	if(TimerAddr->LoopTimes>=TimerAddr->MAXLoopTimes){
        //如果超过最大循环次数，复原循环次数
		TimerAddr->LoopTimes=0;
        //并且返回超时
		return TIMER_TIMEOUT;
	}
	else
		return TIMER_NO_TIMEOUT;
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update)!=RESET){
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        //以下语句如果“不跳过中断”，则执行，功能是：处理一遍时钟循环队列中所有的时钟
        //跳过正在取消的时钟或者正在添加的时钟
		if((StartTimer!=CIRCULARQUEUE_NULL)&&(SkipThisinterrupt==TIMER_NO_SKIP_INTERRUPT)){
			TimerTemp=StartTimer;
			do{
				(((Timer*)(TimerTemp->Data))->Counter)++;//计数值加1？
				TimerTemp=TimerTemp->NextOne;//处理一遍时钟循环队列中所有的时钟
			}while(TimerTemp!=StartTimer);
		}
	}
}
