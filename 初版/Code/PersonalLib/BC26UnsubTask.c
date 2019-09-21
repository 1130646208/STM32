#include "BC26Task.h"

Task BC26UnsubTask;
uint8_t BC26Unsubed=0;
uint8_t BC26UnsubOK=0;
uint8_t BC26UnsubIndex=0;
Timer BC26UnsubTimer={.Using=TIMER_NO_USING};

uint8_t BC26UnsubErrorOut(uint8_t* ErrorTimes)
{
	(*ErrorTimes)++;
	if((*ErrorTimes)>MAX_ERROR_TIMES){
		TimerSub(&BC26UnsubTimer);
		BC26UnsubIndex=6;
		BC26ResetTask.Sleep=TASK_NO_SLEEP;
		(*ErrorTimes)=0;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
uint8_t BC26UnsubResultHandle(void)
{
	static uint8_t ErrorTimes=0;
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		TimerAdd(&BC26UnsubTimer,5000,1);
		if(BC26UnsubOK==1){
			BC26UnsubOK=0;
			DEBUG("Unub OK\r\n");
			TimerSub(&BC26UnsubTimer);
			BC26UnsubIndex++;
			ErrorTimes=0;
			return BC26TX_FINISH;
		}
		else if(TimerLoopTimeOut(&BC26UnsubTimer)==TIMER_TIMEOUT){
			DEBUG("Retry Unsub.\r\n");
			return BC26UnsubErrorOut(&ErrorTimes);
		}
		else
			return BC26TX_WAIT;
	}
	else{
		TimerAdd(&BC26UnsubTimer,500,1);
		if(TimerLoopTimeOut(&BC26UnsubTimer)==TIMER_TIMEOUT){
			DEBUG("Unsub ERROR\r\n");
			return BC26UnsubErrorOut(&ErrorTimes);
		}
		else
			return BC26TX_WAIT;
	}
}
void BC26Unsub(Task *TaskAddr)
{
	if(BC26Unsubed==1)
		BC26UnsubIndex=5;
	switch(BC26UnsubIndex){
		case 0:
			BC26TX.TXRequest(BC26UnsubResultHandle,"AT+QMTUNS=0,1,\"%s/%s\"\r\n",MQTT_STATUS,ClientID);
		break;
		case 1:
			BC26TX.TXRequest(BC26UnsubResultHandle,"AT+QMTUNS=0,1,\"%s/%s\"\r\n",MQTT_BT,ClientID);
		break;
		case 2:
			BC26TX.TXRequest(BC26UnsubResultHandle,"AT+QMTUNS=0,1,\"%s/%s\"\r\n",MQTT_ACTION,ClientID);
		break;
		case 3:
			BC26TX.TXRequest(BC26UnsubResultHandle,"AT+QMTUNS=0,1,\"%s/%s\"\r\n",MQTT_QUERY,ClientID);
		break;
		case 4:
			BC26TX.TXRequest(BC26UnsubResultHandle,"AT+QMTUNS=0,1,\"%s/%s\"\r\n",MQTT_HEART,ClientID);
		break;
		case 5:
			if(BC26ResetTask.Sleep==TASK_SLEEP){
				DEBUG("ALL Unsub OK\r\n");
				if(BC26Unsubed==0)
					BC26Subed=0;
				BC26Unsubed=1;
				WakeUpNextTask(TaskAddr);
			}
			else{
				DEBUG("Unsub TO RESET.\r\n");
			}
			BC26UnsubIndex=0;
			TaskAddr->Sleep=TASK_SLEEP;
		break;
	}
}