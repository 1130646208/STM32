#include "BC26.h"
#include "Delay.h"
#include "BC26Task.h"
#include "Timer.h"

Task BC26ResetTask;

void BC26GpioInit(void)
{
	GPIO_InitTypeDef GPIOInit;
	RCC_APB2PeriphClockCmd(BC26_RESET_PORT_RCC,ENABLE);
	GPIOInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIOInit.GPIO_Pin=BC26_RESET_PIN;
	GPIO_Init(BC26_RESET_PORT,&GPIOInit);
	
	GPIO_SetBits(BC26_RESET_PORT,BC26_RESET_PIN);
	DelayMs(1500);
	GPIO_ResetBits(BC26_RESET_PORT,BC26_RESET_PIN);
	while(strstr(BC26RX.BufferTemp,"Leaving the BROM")==NULL);//检查下RX TX接好了没?
	BC26RX.ATCOMStatus=BC26_ATCOM_ASK;
	while(BC26RX.ATCOMStatus!=BC26_ATCOM_RESPOND){
		UsartSentDatas(USART3,"ATI\r\n");
		DelayMs(100);
	}
}
void BC26ResetMission(Task *TaskAddr)
{
	static Timer CheckTimer={.Using=TIMER_NO_USING};
	if(CheckTimer.Using==TIMER_NO_USING){
		TimerAdd(&CheckTimer,1500,1);
		GPIO_SetBits(BC26_RESET_PORT,BC26_RESET_PIN);
	}
	else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
		TimerSub(&CheckTimer);
		GPIO_ResetBits(BC26_RESET_PORT,BC26_RESET_PIN);
		while(strstr(BC26RX.BufferTemp,"Leaving the BROM")==NULL);
		BC26RX.ATCOMStatus=BC26_ATCOM_ASK;
		while(BC26RX.ATCOMStatus!=BC26_ATCOM_RESPOND){
			UsartSentDatas(USART3,"ATI\r\n");
			DelayMs(100);
		}
		BC26CheckTask.Sleep=TASK_NO_SLEEP;
		TaskAddr->Sleep=TASK_SLEEP;
	}
}

