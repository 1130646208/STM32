#include "BC26Task.h"
#define CARD_CHECK_TIMES 2000
Task BC26CardCheckTask;
uint8_t BC26CardCheckResult=0;
Timer BC26CardCheckTimer={.Using=TIMER_NO_USING};

uint8_t BC26CardCheckErrorOut(uint16_t* ErrorTimes)
{
	(*ErrorTimes)++;
	if((*ErrorTimes)>CARD_CHECK_TIMES){
		TimerSub(&BC26CardCheckTimer);
		BC26CardCheckResult=1;
		BC26ResetTask.Sleep=TASK_NO_SLEEP;
		(*ErrorTimes)=0;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
uint8_t BC26CardCheckResultHandle(void)
{
	static uint16_t ErrorTimes=0;
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		TimerAdd(&BC26CardCheckTimer,1000,1);
		if(strstr(BC26RX.ABuffer,"\n+CPIN: READY\n")!=NULL){
			DEBUG("Card Ready\r\n");
			TimerSub(&BC26CardCheckTimer);
			BC26CardCheckResult=1;
			ErrorTimes=0;
			return BC26TX_FINISH;
		}
		else if(TimerLoopTimeOut(&BC26CardCheckTimer)==TIMER_TIMEOUT){
			DEBUG("Card no Ready\r\n");
			return BC26CardCheckErrorOut(&ErrorTimes);
		}
		else
			return BC26TX_WAIT;
	}
	else
		return BC26CardCheckErrorOut(&ErrorTimes);
}
void BC26CardCheck(Task *TaskAddr)
{
	if(BC26CardCheckResult==0)
		BC26TX.TXRequest(BC26CardCheckResultHandle,"AT+CPIN?\r\n");
	else{
		if(BC26ResetTask.Sleep==TASK_SLEEP)
			WakeUpNextTask(TaskAddr);
		else
			DEBUG("Card Check TO RESET.\r\n");
		BC26CardCheckResult=0;
		TaskAddr->Sleep=TASK_SLEEP;
	}
}