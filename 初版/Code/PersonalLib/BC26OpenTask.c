#include "BC26Task.h"

char IPAddr[16]={"47.106.249.19"};//5
char Port[6]={"61613"};

Task BC26OpenTask;
uint8_t BC26OpenIndex=0;
Timer BC26OpenTimer={.Using=TIMER_NO_USING};
uint8_t BC26SuccessOpen=0;

uint8_t BC26OpenErrorOut(uint8_t* ErrorTimes)
{
	(*ErrorTimes)++;
	if((*ErrorTimes)>MAX_ERROR_TIMES){
		TimerSub(&BC26OpenTimer);
		BC26OpenIndex=2;
		BC26ResetTask.Sleep=TASK_NO_SLEEP;
		(*ErrorTimes)=0;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
uint8_t BC26OpenResultHandle(void)
{
	static uint8_t ErrorTimes=0;
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		switch(BC26OpenIndex){
			case 0:
				if(strstr(BC26RX.ABuffer,IPAddr)!=NULL){
					DEBUG("Already Open\r\n");
					BC26SuccessOpen=1;
					BC26OpenIndex=2;
				}
				else{
					DEBUG("Do no Open\r\n");
					BC26SuccessOpen=0;
					BC26OpenIndex=1;
				}
				ErrorTimes=0;
				return BC26TX_FINISH;
			break;
			case 1:
				TimerAdd(&BC26OpenTimer,10000,1);
				if(BC26SuccessOpen==1){
					DEBUG("Success Open\r\n");
					TimerSub(&BC26OpenTimer);
					BC26OpenIndex=2;
					ErrorTimes=0;
					return BC26TX_FINISH;
				}
				else if(TimerLoopTimeOut(&BC26OpenTimer)==TIMER_TIMEOUT){
					DEBUG("Open Fail\r\n");
					return BC26OpenErrorOut(&ErrorTimes);
				}
				else
					return BC26TX_WAIT;
		}
	}
	else
		return BC26OpenErrorOut(&ErrorTimes);
}
void BC26Open(Task *TaskAddr)
{
	switch(BC26OpenIndex){
		case 0:
			BC26TX.TXRequest(BC26OpenResultHandle,"AT+QMTOPEN?\r\n");
		break;
		case 1:
			BC26TX.TXRequest(BC26OpenResultHandle,"AT+QMTOPEN=0,\"%s\",%s\r\n",IPAddr,Port);
		break;
		case 2:
			if(BC26ResetTask.Sleep==TASK_SLEEP)
				WakeUpNextTask(TaskAddr);
			else
				DEBUG("Open TO RESET.\r\n");
			BC26OpenIndex=0;
			TaskAddr->Sleep=TASK_SLEEP;
		break;
	}
}