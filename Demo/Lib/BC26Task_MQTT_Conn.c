#include "BC26Task_MQTT_Conn.h"
#include "BC26Task_MQTT_Open.h"
#include "BC26Task_Device_Check.h"
#include "BC26TASK_MQTT_Pub.h"
#include "BC26TASK_MQTT_Sub.h"
#include "BC26TX.h"
#include "BC26RX.h"
#include "BC26.h"
#include "Timer.h"
#include "DEBUG.h"
#include "Flash.h"

uint8_t BC26MQTTConned=BC26MQTT_NO_CONN;
Task BC26MQTTConnTask;

const uint8_t COMMAND_MQTT_CONN_CHECK[]={"AT+QMTCONN?\r\n"};
const uint8_t COMMAND_MQTT_CONN_SET1[]={"AT+QMTCONN=0,\""};
const uint8_t COMMAND_MQTT_CONN_SET2[]={"\",\""};
const uint8_t COMMAND_MQTT_CONN_SET3[]={"\"\r\n"};

void BC26MQTTConn(Task *TaskAddr)
{
	static Timer CheckTimer;
	static uint8_t CommandAsk=BC26_COMMAND_READY_ASK;
	static uint8_t Index=0;
	uint8_t i,j,Row;
	if(BC26MQTTOpened==BC26MQTT_OPEN){
		if((CommandAsk==BC26_COMMAND_READY_ASK)&&(BC26TxBuffersStatus==BC26TX_BUFFER_EMPTY)){
			switch(Index){
				case 0:
								for(i=0;COMMAND_MQTT_CONN_CHECK[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_MQTT_CONN_CHECK[i];
								break;
				case 1:
								for(i=0;COMMAND_MQTT_CONN_SET1[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_MQTT_CONN_SET1[i];
								for(j=0;DevicesID[j]!='\0';i++,j++)
									BC26TxBuffers[i]=DevicesID[j];
								for(j=0;COMMAND_MQTT_CONN_SET2[j]!='\0';i++,j++)
									BC26TxBuffers[i]=COMMAND_MQTT_CONN_SET2[j];
								for(j=0;Username[j]!='\0';i++,j++)
									BC26TxBuffers[i]=Username[j];
								for(j=0;COMMAND_MQTT_CONN_SET2[j]!='\0';i++,j++)
									BC26TxBuffers[i]=COMMAND_MQTT_CONN_SET2[j];
								for(j=0;Password[j]!='\0';i++,j++)
									BC26TxBuffers[i]=Password[j];
								for(j=0;COMMAND_MQTT_CONN_SET3[j]!='\0';i++,j++)
									BC26TxBuffers[i]=COMMAND_MQTT_CONN_SET3[j];
								break;
			}
			BC26TxBuffers[i]='\0';
			BC26TxBuffersStatus=BC26TX_BUFFER_NEW;
			CommandAsk=BC26_COMMAND_ASKING;
			TimerAdd(&CheckTimer,500,1);
		}
		else if(CommandAsk==BC26_COMMAND_ASKING){
			if(BC26TxBuffersStatus==BC26TX_BUFFER_READY){
				if((BC26RxATStartRow+1)==BC26RX_MAXROW)
					Row=0;
				else
					Row=BC26RxATStartRow+1;
				if(Index==0){
					if(BC26RxBuffer[Row][12]!='3'){
						DEBUG("MQTT NO CONN.\r\n");
						BC26MQTTConned=BC26MQTT_NO_CONN;
						Index=1;
						TimerSub(&CheckTimer);
						TimerAdd(&CheckTimer,500,1);
					}
					else{
						if(BC26MQTTConned==BC26MQTT_NO_CONN)
							DEBUG("MQTT CONN.\r\n");
						BC26MQTTConned=BC26MQTT_CONN;
						if((BC26MQTTSubTask.Sleep==TASK_SLEEP)&&(BC26MQTTSubed==BC26MQTT_NO_SUB)){
							BC26MQTTSubTask.Sleep=TASK_NO_SLEEP;
						}
						if(StringEqual(ClientID,"default")==0){
							BC26MQTTSPubTask.Sleep=TASK_NO_SLEEP;
						}
						TimerSub(&CheckTimer);
						TimerAdd(&CheckTimer,500,1);
					}
				}
				else{
					Index=0;
					TimerSub(&CheckTimer);
					TimerAdd(&CheckTimer,3000,1);
				}
				BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
				CommandAsk=BC26_COMMAND_WAIT_ASK;
			}
			else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
				BC26ConnectStatus=BC26DISCONNECT;
				BC26MQTTConned=BC26MQTT_NO_CONN;
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
		BC26MQTTConned=BC26MQTT_NO_CONN;
		CommandAsk=BC26_COMMAND_READY_ASK;
		if(CheckTimer.Using==TIMER_USING)
			TimerSub(&CheckTimer);
	}
}
