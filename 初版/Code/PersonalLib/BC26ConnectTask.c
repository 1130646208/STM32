#include "BC26Task.h"

char Password[17]={"password"};
char UserName[17]={"admin"};

Task BC26ConnectTask;
uint8_t BC26ConnectIndex=0;
Timer BC26ConnectTimer={.Using=TIMER_NO_USING};
uint8_t BC26SuccessConnect=0;

uint8_t BC26ConnectErrorOut(uint8_t* ErrorTimes)
{
	(*ErrorTimes)++;
	if((*ErrorTimes)>MAX_ERROR_TIMES){
		TimerSub(&BC26ConnectTimer);
		BC26ConnectIndex=2;
		BC26ResetTask.Sleep=TASK_NO_SLEEP;
		(*ErrorTimes)=0;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
uint8_t BC26ConnectResultHandle(void)
{
	static uint8_t ErrorTimes=0;
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		switch(BC26ConnectIndex){
			case 0:
				if(strstr(BC26RX.ABuffer,"+QMTCONN: 0,3")!=NULL){
					DEBUG("Already Connect\r\n");
					BC26SuccessConnect=1;
					BC26ConnectIndex=2;
				}
				else{
					DEBUG("Do no Connect\r\n");
					BC26SuccessConnect=0;
					BC26ConnectIndex=1;
				}
				ErrorTimes=0;
				return BC26TX_FINISH;
			break;
			case 1:
				TimerAdd(&BC26ConnectTimer,10000,1);//Á¬½ÓµÈ´ý10Ãë
				if(BC26SuccessConnect==1){
					DEBUG("Connect Success\r\n");
					TimerSub(&BC26ConnectTimer);
					BC26ConnectIndex=2;
					ErrorTimes=0;
					return BC26TX_FINISH;
				}
				else if(TimerLoopTimeOut(&BC26ConnectTimer)==TIMER_TIMEOUT){
					DEBUG("Connect Fail\r\n");
					return BC26ConnectErrorOut(&ErrorTimes);
				}
				else
					return BC26TX_WAIT;
		}
	}
	else
		return BC26ConnectErrorOut(&ErrorTimes);
}
void BC26Connect(Task *TaskAddr)
{
	switch(BC26ConnectIndex){
		case 0:
			BC26TX.TXRequest(BC26ConnectResultHandle,"AT+QMTCONN?\r\n");
		break;
		case 1:
			BC26TX.TXRequest(BC26ConnectResultHandle,"AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n",DevicesID,UserName,Password);
		break;
		case 2:
			if(BC26ResetTask.Sleep==TASK_SLEEP)
				WakeUpNextTask(TaskAddr);
			else
				DEBUG("Connect TO RESET.\r\n");
			BC26ConnectIndex=0;
			TaskAddr->Sleep=TASK_SLEEP;
		break;
	}
}