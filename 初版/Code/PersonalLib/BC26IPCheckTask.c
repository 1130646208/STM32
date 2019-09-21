#include "BC26Task.h"

Task BC26IPCheckTask;
uint8_t BC26IPCheckResult=0;
Timer BC26IPCheckTimer={.Using=TIMER_NO_USING};

uint8_t BC26IPCheckResultHandle(void);

uint8_t BC26IPCheckErrorOut(uint8_t* ErrorTimes)
{
	(*ErrorTimes)++;
	if((*ErrorTimes)>MAX_ERROR_TIMES){
		TimerSub(&BC26IPCheckTimer);
		BC26IPCheckResult=1;
		BC26ResetTask.Sleep=TASK_NO_SLEEP;
		(*ErrorTimes)=0;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
uint8_t BC26IPCheckResultHandle(void)
{
	static uint8_t ErrorTimes=0;
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		TimerAdd(&BC26IPCheckTimer,10000,1);
		if(strstr(BC26RX.ABuffer,"\n+CGATT: 1\n")!=NULL){
			DEBUG("IP Ready\r\n");
			TimerSub(&BC26IPCheckTimer);
			BC26IPCheckResult=1;
			ErrorTimes=0;
			return BC26TX_FINISH;
		}
		else if(TimerLoopTimeOut(&BC26IPCheckTimer)==TIMER_TIMEOUT){
			DEBUG("IP No Ready\r\n");
			return BC26IPCheckErrorOut(&ErrorTimes);
		}
		else
			return BC26TX_WAIT;
	}
	else
		return BC26IPCheckErrorOut(&ErrorTimes);
}
void BC26IPCheck(Task *TaskAddr)
{
	if(BC26IPCheckResult==0)
		BC26TX.TXRequest(BC26IPCheckResultHandle,"AT+CGATT?\r\n");
	else{
		if(BC26ResetTask.Sleep==TASK_SLEEP)
			WakeUpNextTask(TaskAddr);
		else
			DEBUG("IP Check TO RESET.\r\n");
		BC26IPCheckResult=0;
		TaskAddr->Sleep=TASK_SLEEP;
	}
}