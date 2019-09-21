#include "BC26Task_MQTT_Open.h"
#include "BC26Task_GetIP_Check.h"
#include "BC26Task_MQTT_Init.h"
#include "BC26Task_Device_Check.h"
#include "BC26Task_MQTT_Conn.h"
#include "BC26TX.h"
#include "BC26RX.h"
#include "BC26.h"
#include "Timer.h"
#include "Flash.h"
#include "DEBUG.h"

uint8_t BC26MQTTOpened=BC26MQTT_NO_OPEN;
Task BC26MQTTOpenTask;

const uint8_t COMMAND_MQTT_OPEN_CHECK[]={"AT+QMTOPEN?\r\n"};
const uint8_t COMMAND_MQTT_OPEN_SET1[]={"AT+QMTOPEN=0,\""};
const uint8_t COMMAND_MQTT_OPEN_SET2[]={"\",\""};
const uint8_t COMMAND_MQTT_OPEN_SET3[]={"\"\r\n"};

void BC26MQTTOpen(Task *TaskAddr)
{
	static Timer CheckTimer;
	static uint8_t CommandAsk=BC26_COMMAND_READY_ASK;
	static uint8_t Index=0;
	static uint8_t FailTimes=0;
	uint8_t i,j,Row;
	if(BC26MQTTInited==BC26MQTT_INIT){
		if((CommandAsk==BC26_COMMAND_READY_ASK)&&(BC26TxBuffersStatus==BC26TX_BUFFER_EMPTY)){
			switch(Index){
				case 0:
								for(i=0;COMMAND_MQTT_OPEN_CHECK[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_MQTT_OPEN_CHECK[i];
								break;
				case 1:
								for(i=0;COMMAND_IP_ACT[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_IP_ACT[i];
								break;
				case 2:
								for(i=0;COMMAND_MQTT_OPEN_SET1[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_MQTT_OPEN_SET1[i];
								for(j=0;IPAddr[j]!='\0';i++,j++)
									BC26TxBuffers[i]=IPAddr[j];
								for(j=0;COMMAND_MQTT_OPEN_SET2[j]!='\0';i++,j++)
									BC26TxBuffers[i]=COMMAND_MQTT_OPEN_SET2[j];
								for(j=0;Port[j]!='\0';i++,j++)
									BC26TxBuffers[i]=Port[j];
								for(j=0;COMMAND_MQTT_OPEN_SET3[j]!='\0';i++,j++)
									BC26TxBuffers[i]=COMMAND_MQTT_OPEN_SET3[j];
								break;
				case 3:
								for(i=0;COMMAND_IP_DEACT[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_IP_DEACT[i];
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
					if(BC26RxBuffer[Row][0]!='+'){
						DEBUG("MQTT NO OPEN.\r\n");
						DEBUG(BC26RxBuffer[Row]);
						BC26MQTTOpened=BC26MQTT_NO_OPEN;
						Index=1;
						FailTimes++;
						TimerSub(&CheckTimer);
						TimerAdd(&CheckTimer,500,1);
					}
					else{
						if(BC26MQTTOpened==BC26MQTT_NO_OPEN)
							DEBUG("MQTT OPEN.\r\n");
						BC26MQTTOpened=BC26MQTT_OPEN;
						FailTimes=0;
						if(BC26MQTTConnTask.Sleep==TASK_SLEEP){
							BC26MQTTConnTask.Sleep=TASK_NO_SLEEP;
						}
						TimerSub(&CheckTimer);
						TimerAdd(&CheckTimer,500,1);
					}
				}
				else if(Index==1){
					if(FailTimes>=5){
						FailTimes=0;
						Index=3;
					}
					else{
						Index=2;
					}
					TimerSub(&CheckTimer);
					TimerAdd(&CheckTimer,1000,1);
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
				BC26MQTTOpened=BC26MQTT_NO_OPEN;
				Index=0;
				FailTimes=0;
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
		FailTimes=0;
		BC26MQTTOpened=BC26MQTT_NO_OPEN;
		CommandAsk=BC26_COMMAND_READY_ASK;
		if(CheckTimer.Using==TIMER_USING)
			TimerSub(&CheckTimer);
	}
}
