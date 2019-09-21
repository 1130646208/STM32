#include "BC26TASK_MQTT_Unsub.h"
#include "BC26TASK_MQTT_Sub.h"
#include "BC26Task_Device_Check.h"
#include "BC26TASK_MQTT_Pub.h"
#include "BC26TX.h"
#include "DEBUG.h"
#include "BC26RX.h"
#include "BC26.h"
#include "Timer.h"
#include "Flash.h"

Task BC26MQTTUnsubTask;

const uint8_t COMMAND_MQTT_UNSUB_SET1[]={"AT+QMTUNS=0,1,\""};
const uint8_t COMMAND_MQTT_UNSUB_SET2[]={"\"\r\n"};

void UnsubTopic(const uint8_t *Topic,uint8_t *i);
extern uint8_t ClientIDTemp[17];
void BC26MQTTUnsub(Task *TaskAddr)
{
	static Timer CheckTimer;
	static uint8_t CommandAsk=BC26_COMMAND_READY_ASK;
	static uint8_t Index=0;
	uint8_t i,Row;
	if(BC26MQTTSubed==BC26MQTT_SUB){
		if((CommandAsk==BC26_COMMAND_READY_ASK)&&(BC26TxBuffersStatus==BC26TX_BUFFER_EMPTY)){
			if(StringEqual(ClientID,"default")==1)
				Index=5;
			switch(Index){
				case 0:
								UnsubTopic(MQTT_SUB_STATUS,&i);
								break;
				case 1:
								UnsubTopic(MQTT_SUB_BT,&i);
								break;
				case 2:
								UnsubTopic(MQTT_SUB_ACTION,&i);
								break;
				case 3:
								UnsubTopic(MQTT_SUB_QUERY,&i);
								break;
				case 4:
								UnsubTopic(MQTT_SUB_HEART,&i);
								break;
			}
			if(Index!=5){
				BC26TxBuffers[i]='\0';
				BC26TxBuffersStatus=BC26TX_BUFFER_NEW;
				CommandAsk=BC26_COMMAND_ASKING;
				TimerAdd(&CheckTimer,500,1);
			}
			else
				Index=0;
		}
		else if(CommandAsk==BC26_COMMAND_ASKING){
			if(BC26TxBuffersStatus==BC26TX_BUFFER_READY){
				if((BC26RxATStartRow+1)==BC26RX_MAXROW)
					Row=0;
				else
					Row=BC26RxATStartRow+1;
				if(BC26RxBuffer[Row][0]=='O')
					Index++;
				BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
				TimerSub(&CheckTimer);
				CommandAsk=BC26_COMMAND_WAIT_ASK;
				TimerAdd(&CheckTimer,100,1);
			}
			else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
				BC26ConnectStatus=BC26DISCONNECT;
				Index=0;
				BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
				CommandAsk=BC26_COMMAND_READY_ASK;
				TimerSub(&CheckTimer);
				TaskAddr->Sleep=TASK_SLEEP;
			}
		}
		else if((CommandAsk==BC26_COMMAND_WAIT_ASK)&&(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT)){
			TimerSub(&CheckTimer);
			CommandAsk=BC26_COMMAND_READY_ASK;
		}
	}
	else{
		TaskAddr->Sleep=TASK_SLEEP;
		Index=0;
		CommandAsk=BC26_COMMAND_READY_ASK;
		if(CheckTimer.Using==TIMER_USING)
			TimerSub(&CheckTimer);
	}
}
void UnsubTopic(const uint8_t *Topic,uint8_t *i)
{
	uint8_t j;
	for((*i)=0;COMMAND_MQTT_UNSUB_SET1[(*i)]!='\0';(*i)++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_UNSUB_SET1[(*i)];
	for(j=0;Topic[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=Topic[j];
	for(j=0;ClientID[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=ClientID[j];
	for(j=0;COMMAND_MQTT_UNSUB_SET2[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_UNSUB_SET2[j];
}
