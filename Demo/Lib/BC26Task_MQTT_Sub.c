#include "BC26TASK_MQTT_Sub.h"
#include "BC26Task_MQTT_Conn.h"
#include "BC26TASK_MQTT_Pub.h"
#include "BC26Task_Device_Check.h"
#include "BC26TX.h"
#include "BC26RX.h"
#include "BC26.h"
#include "Timer.h"
#include "Flash.h"
#include "DEBUG.h"

uint8_t BC26MQTTSubed=BC26MQTT_NO_SUB;
Task BC26MQTTSubTask;

const uint8_t COMMAND_MQTT_SUB_SET1[]={"AT+QMTSUB=0,1,\""};
const uint8_t COMMAND_MQTT_SUB_SET2[]={"\","};
const uint8_t COMMAND_MQTT_SUB_SET3[]={"\r\n"};

void SubTopic(const uint8_t *Topic,uint8_t *Id,uint8_t *i,uint8_t QOS);

void BC26MQTTSub(Task *TaskAddr)
{
	static Timer CheckTimer;
	static uint8_t CommandAsk=BC26_COMMAND_READY_ASK;
	static uint8_t Index=0;
	uint8_t i,Row;
	if(BC26MQTTConned==BC26MQTT_CONN){
		if((CommandAsk==BC26_COMMAND_READY_ASK)&&(BC26TxBuffersStatus==BC26TX_BUFFER_EMPTY)){
			switch(Index){
				case 0:
								SubTopic(MQTT_SUB_ACTIVATE,DevicesID,&i,'1');
								DEBUG("SUB 0\r\n");
								break;
				case 1:
								SubTopic(MQTT_SUB_STATUS,ClientID,&i,'1');
								DEBUG("SUB 1\r\n");
								break;
				case 2:
								SubTopic(MQTT_SUB_BT,ClientID,&i,'1');
								DEBUG("SUB 2\r\n");
								break;
				case 3:
								SubTopic(MQTT_SUB_ACTION,ClientID,&i,'1');
								DEBUG("SUB 3\r\n");
								break;
				case 4:
								SubTopic(MQTT_SUB_QUERY,ClientID,&i,'1');
								DEBUG("SUB 4\r\n");
								break;
				case 5:
								SubTopic(MQTT_SUB_HEART,ClientID,&i,'1');
								DEBUG("SUB 5\r\n");
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
				if(BC26RxBuffer[Row][0]=='O'){
					Index++;
				}
				BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
				TimerSub(&CheckTimer);
				CommandAsk=BC26_COMMAND_WAIT_ASK;
				TimerAdd(&CheckTimer,500,1);
			}
			else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
				BC26ConnectStatus=BC26DISCONNECT;
				BC26MQTTSubed=BC26MQTT_NO_SUB;
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
		BC26MQTTSubed=BC26MQTT_NO_SUB;
		Index=0;
		CommandAsk=BC26_COMMAND_READY_ASK;
		if(CheckTimer.Using==TIMER_USING)
			TimerSub(&CheckTimer);
	}
}
void SubTopic(const uint8_t *Topic,uint8_t *Id,uint8_t *i,uint8_t QOS)
{
	uint8_t j;
	for((*i)=0;COMMAND_MQTT_SUB_SET1[(*i)]!='\0';(*i)++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_SUB_SET1[(*i)];
	for(j=0;Topic[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=Topic[j];
	for(j=0;Id[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=Id[j];
	for(j=0;COMMAND_MQTT_SUB_SET2[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_SUB_SET2[j];
	BC26TxBuffers[(*i)]=QOS;
	(*i)++;
	for(j=0;COMMAND_MQTT_SUB_SET3[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_SUB_SET3[j];
}
