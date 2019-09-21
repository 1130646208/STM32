#include "BC26Task.h"

Task BC26SubTask;
uint8_t BC26Subed=0;
uint8_t BC26SubOK=0;
uint8_t BC26SubIndex=0;
Timer BC26SubTimer={.Using=TIMER_NO_USING};

uint8_t BC26SubErrorOut(uint8_t* ErrorTimes)
{
	(*ErrorTimes)++;
	if((*ErrorTimes)>MAX_ERROR_TIMES){
		TimerSub(&BC26SubTimer);
		BC26SubIndex=6;
		BC26ResetTask.Sleep=TASK_NO_SLEEP;
		(*ErrorTimes)=0;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
uint8_t BC26SubResultHandle(void)
{
	static uint8_t ErrorTimes=0;
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		TimerAdd(&BC26SubTimer,5000,1);
		if(BC26SubOK==1){
			BC26SubOK=0;
			DEBUG("Sub OK\r\n");
			TimerSub(&BC26SubTimer);
			BC26SubIndex++;
			ErrorTimes=0;
			return BC26TX_FINISH;
		}
		else if(TimerLoopTimeOut(&BC26SubTimer)==TIMER_TIMEOUT){
			DEBUG("Retry Sub.\r\n");
			return BC26SubErrorOut(&ErrorTimes);
		}
		else
			return BC26TX_WAIT;
	}
	else{
		TimerAdd(&BC26SubTimer,500,1);
		if(TimerLoopTimeOut(&BC26SubTimer)==TIMER_TIMEOUT){
			DEBUG("SUB ERROR\r\n");
			return BC26SubErrorOut(&ErrorTimes);
		}
		else
			return BC26TX_WAIT;
	}
}
void BC26Sub(Task *TaskAddr)
{
	if(BC26Subed==1)
		BC26SubIndex=6;
	switch(BC26SubIndex){
		case 0:
			BC26TX.TXRequest(BC26SubResultHandle,"AT+QMTSUB=0,1,\"%s/%s\",%d\r\n",MQTT_ACTIVATE,DevicesID,BC26_QOS);
		break;
		case 1:
			BC26TX.TXRequest(BC26SubResultHandle,"AT+QMTSUB=0,1,\"%s/%s\",%d\r\n",MQTT_STATUS,ClientID,BC26_QOS);
		break;
		case 2:
			BC26TX.TXRequest(BC26SubResultHandle,"AT+QMTSUB=0,1,\"%s/%s\",%d\r\n",MQTT_BT,ClientID,BC26_QOS);
		break;
		case 3:
			BC26TX.TXRequest(BC26SubResultHandle,"AT+QMTSUB=0,1,\"%s/%s\",%d\r\n",MQTT_ACTION,ClientID,BC26_QOS);
		break;
		case 4:
			BC26TX.TXRequest(BC26SubResultHandle,"AT+QMTSUB=0,1,\"%s/%s\",%d\r\n",MQTT_QUERY,ClientID,BC26_QOS);
		break;
		case 5:
			BC26TX.TXRequest(BC26SubResultHandle,"AT+QMTSUB=0,1,\"%s/%s\",%d\r\n",MQTT_HEART,ClientID,BC26_QOS);
		break;
		case 6:
			if(BC26ResetTask.Sleep==TASK_SLEEP){
				DEBUG("ALL Sub OK\r\n");
				BC26Subed=1;
				WakeUpNextTask(TaskAddr);
			}
			else{
				DEBUG("Sub TO RESET.\r\n");
				//BC26Subed=0;
			}
			BC26SubIndex=0;
			TaskAddr->Sleep=TASK_SLEEP;
		break;
	}
}