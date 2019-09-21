#include "BC26TASK_MQTT_Pub.h"
#include "BC26TASK_MQTT_Sub.h"
#include "BC26TASK_MQTT_Unsub.h"
#include "BC26Task_Device_Check.h"
#include "BC26Task_MQTT_Conn.h"
#include "BC26TX.h"
#include "BC26RX.h"
#include "BC26.h"
#include "DEBUG.h"
#include "Timer.h"
#include "Flash.h"

uint8_t PublicQueue[MQTT_PUB_MAX]={BC26MQTT_NO_PUB};
uint8_t PublicQueueData[MQTT_PUB_MAX];

const uint8_t COMMAND_MQTT_PUB_SET1[]={"AT+QMTPUB=0,1,"};
const uint8_t COMMAND_MQTT_PUB_Msg1[2][2]={{"0"},{"1"}};
const uint8_t COMMAND_MQTT_PUB_Msg2[6][3]={{"10"},{"11"},{"20"},{"21"},{"30"},{"31"}};
const uint8_t COMMAND_MQTT_PUB_Msg3[3][4]={{"100"},{"101"},{"102"}};
const uint8_t COMMAND_MQTT_PUB_SET2[]={",\""};
const uint8_t COMMAND_MQTT_PUB_SET3[]={"\",\""};
const uint8_t COMMAND_MQTT_PUB_SET4[]={"\"\r\n"};

void PubTopic(uint8_t QOS,uint8_t Retain,const uint8_t *Topic,const uint8_t *Msg,uint8_t *i);

Task BC26MQTTSPubTask;

void BC26MQTTPub(Task *TaskAddr)
{
	static Timer CheckTimer;
	static uint8_t CommandAsk=BC26_COMMAND_READY_ASK;
	uint8_t i,Row,NeedPub=0;
	if(BC26MQTTConned==BC26MQTT_CONN){
		if((CommandAsk==BC26_COMMAND_READY_ASK)&&(BC26TxBuffersStatus==BC26TX_BUFFER_EMPTY)){
			if(PublicQueue[MQTT_PUB_ACTIVATE_INDEX]==BC26MQTT_PUB){
				PubTopic('1','0',MQTT_PUB_ACTIVATE,COMMAND_MQTT_PUB_Msg1[PublicQueueData[MQTT_PUB_ACTIVATE_INDEX]],&i);
				NeedPub=1;
			}
			else if(PublicQueue[MQTT_PUB_STATUS_INDEX]==BC26MQTT_PUB){
				PubTopic('1','0',MQTT_PUB_STATUS,COMMAND_MQTT_PUB_Msg2[PublicQueueData[MQTT_PUB_STATUS_INDEX]],&i);
				NeedPub=1;
			}
			else if(PublicQueue[MQTT_PUB_BT_INDEX]==BC26MQTT_PUB){
				PubTopic('1','0',MQTT_PUB_BT,COMMAND_MQTT_PUB_Msg1[PublicQueueData[MQTT_PUB_BT_INDEX]],&i);
				NeedPub=1;
			}
			else if(PublicQueue[MQTT_PUB_ACTION_INDEX]==BC26MQTT_PUB){
				PubTopic('1','0',MQTT_PUB_ACTION,COMMAND_MQTT_PUB_Msg1[PublicQueueData[MQTT_PUB_ACTION_INDEX]],&i);
				NeedPub=1;
			}
			else if(PublicQueue[MQTT_PUB_REPORT_INDEX]==BC26MQTT_PUB){
				PubTopic('1','0',MQTT_PUB_REPORT,COMMAND_MQTT_PUB_Msg3[PublicQueueData[MQTT_PUB_REPORT_INDEX]],&i);
				NeedPub=1;
			}
			else if(PublicQueue[MQTT_PUB_HEART_INDEX]==BC26MQTT_PUB){
				PubTopic('1','0',MQTT_PUB_HEART,COMMAND_MQTT_PUB_Msg1[PublicQueueData[MQTT_PUB_HEART_INDEX]],&i);
				NeedPub=1;
			}
			if(NeedPub==1){
				BC26TxBuffers[i]='\0';
				DEBUG("PUB:");
				DEBUG(BC26TxBuffers);
				BC26TxBuffersStatus=BC26TX_BUFFER_NEW;
				CommandAsk=BC26_COMMAND_ASKING;
				TimerAdd(&CheckTimer,500,1);
			}
		}
		else if(CommandAsk==BC26_COMMAND_ASKING){
			if(BC26TxBuffersStatus==BC26TX_BUFFER_READY){
				if((BC26RxATStartRow+1)==BC26RX_MAXROW)
					Row=0;
				else
					Row=BC26RxATStartRow+1;
				if(BC26RxBuffer[Row][0]=='O'){
					if(PublicQueue[MQTT_PUB_ACTIVATE_INDEX]==BC26MQTT_PUB){
						PublicQueue[MQTT_PUB_ACTIVATE_INDEX]=BC26MQTT_NO_PUB;
						DEBUG("PUB MQTT_PUB_ACTIVAT OK.\r\n");
					}
					else if(PublicQueue[MQTT_PUB_STATUS_INDEX]==BC26MQTT_PUB){
						PublicQueue[MQTT_PUB_STATUS_INDEX]=BC26MQTT_NO_PUB;
						DEBUG("PUB MQTT_PUB_STATUS OK.\r\n");
					}
					else if(PublicQueue[MQTT_PUB_BT_INDEX]==BC26MQTT_PUB){
						PublicQueue[MQTT_PUB_BT_INDEX]=BC26MQTT_NO_PUB;
						DEBUG("PUB MQTT_PUB_BT OK.\r\n");
					}
					else if(PublicQueue[MQTT_PUB_ACTION_INDEX]==BC26MQTT_PUB){
						PublicQueue[MQTT_PUB_ACTION_INDEX]=BC26MQTT_NO_PUB;
						DEBUG("PUB MQTT_PUB_ACTION OK.\r\n");
					}
					else if(PublicQueue[MQTT_PUB_REPORT_INDEX]==BC26MQTT_PUB){
						PublicQueue[MQTT_PUB_REPORT_INDEX]=BC26MQTT_NO_PUB;
						DEBUG("PUB MQTT_PUB_REPORT OK.\r\n");
					}
					else if(PublicQueue[MQTT_PUB_HEART_INDEX]==BC26MQTT_PUB){
						PublicQueue[MQTT_PUB_HEART_INDEX]=BC26MQTT_NO_PUB;
						DEBUG("PUB MQTT_PUB_HEART OK.\r\n");
					}
				}
				BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
				TimerSub(&CheckTimer);
				CommandAsk=BC26_COMMAND_WAIT_ASK;
				TimerAdd(&CheckTimer,200,1);
			}
			else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
				BC26ConnectStatus=BC26DISCONNECT;
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
		CommandAsk=BC26_COMMAND_READY_ASK;
		if(CheckTimer.Using==TIMER_USING)
			TimerSub(&CheckTimer);
	}
}
void PubTopic(uint8_t QOS,uint8_t Retain,const uint8_t *Topic,const uint8_t *Msg,uint8_t *i)
{
	uint8_t j;
	for((*i)=0;COMMAND_MQTT_PUB_SET1[(*i)]!='\0';(*i)++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_PUB_SET1[(*i)];
	BC26TxBuffers[(*i)]=QOS;
	(*i)++;
	BC26TxBuffers[(*i)]=',';
	(*i)++;
	BC26TxBuffers[(*i)]=Retain;
	(*i)++;
	for(j=0;COMMAND_MQTT_PUB_SET2[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_PUB_SET2[j];
	for(j=0;Topic[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=Topic[j];
	for(j=0;ClientID[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=ClientID[j];
	for(j=0;COMMAND_MQTT_PUB_SET3[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_PUB_SET3[j];
	for(j=0;Msg[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=Msg[j];
	for(j=0;COMMAND_MQTT_PUB_SET4[j]!='\0';(*i)++,j++)
		BC26TxBuffers[(*i)]=COMMAND_MQTT_PUB_SET4[j];
}
