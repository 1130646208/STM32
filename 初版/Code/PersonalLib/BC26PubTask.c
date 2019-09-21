#include "BC26Task.h"

Task BC26PubTask;
uint8_t BC26CanPub=1;
uint8_t BC26PubOK=0;
uint8_t BC26PubIndex=0;
uint8_t pub_times=0;

Timer BC26PubTimer={.Using=TIMER_NO_USING};

MQTTPublishTypedef MQTTPublish[6]={{.PublishOrNo=0},{.PublishOrNo=1},{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0}};

uint8_t BC26PubErrorOut(uint8_t* ErrorTimes)
{
	(*ErrorTimes)++;
	if((*ErrorTimes)>MAX_ERROR_TIMES){
		TimerSub(&BC26PubTimer);
		BC26PubIndex=6;
		BC26ResetTask.Sleep=TASK_NO_SLEEP;
		(*ErrorTimes)=0;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
uint8_t BC26PubResultHandle(void)
{
	static uint8_t ErrorTimes=0;
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		TimerAdd(&BC26PubTimer,5000,1);
		if(BC26PubOK==1){
			BC26PubOK=0;
			DEBUG("Pub OK\r\n");
			MQTTPublish[BC26PubIndex].PublishOrNo=0;
			TimerSub(&BC26PubTimer);
			BC26PubIndex++;
			ErrorTimes=0;
			return BC26TX_FINISH;
		}
		else if(TimerLoopTimeOut(&BC26PubTimer)==TIMER_TIMEOUT){
			DEBUG("Retry Pub.\r\n");
			return BC26PubErrorOut(&ErrorTimes);
		}
		else
			return BC26TX_WAIT;
	}
	else{
		TimerAdd(&BC26PubTimer,500,1);
		if(TimerLoopTimeOut(&BC26PubTimer)==TIMER_TIMEOUT){
			DEBUG("PUB ERROR\r\n");
			return BC26PubErrorOut(&ErrorTimes);
		}
		else
			return BC26TX_WAIT;
	}
}
void BC26Pub(Task *TaskAddr)
{
	uint8_t i;
	if(BC26CanPub==0)
		BC26PubIndex=6;
	else{
		for(i=0;i<6;i++){
			if(MQTTPublish[i].PublishOrNo==1){
				BC26PubIndex=i;
				break;
			}
			else
				BC26PubIndex=6;
		}
	}
	switch(BC26PubIndex){
        //                                            +QMTPUB:0,1,<qos>,0,      "<topic>","<msg>"
		case 0:
			if(MQTTPublish[BC26PubIndex].PublishOrNo==1){
				BC26TX.TXRequest(BC26PubResultHandle,"AT+QMTPUB=0,1,%d,0,\"%s/result/%s\",\"%d\"\r\n",BC26_QOS,MQTT_ACTIVATE,DevicesID,MQTTPublish[BC26PubIndex].Data);
                //                                         BC26_QOS,    MQTT_ACTIVATE,DevicesID,MQTTPublish[BC26PubIndex].Data
			pub_times++;
                break;
			}
			else
				BC26PubIndex++;
		case 1:
			if(MQTTPublish[BC26PubIndex].PublishOrNo==1){
			//BC26TX.TXRequest(BC26PubResultHandle,"AT+QMTPUB=0,1,%d,0,\"%s/result/%s\",\"%d\"\r\n",BC26_QOS,MQTT_STATUS,ClientID,MQTTPublish[BC26PubIndex].Data);
                BC26TX.TXRequest(BC26PubResultHandle,"AT+QMTPUB=0,1,%d,0,\"%s\",\"%s\"\r\n",BC26_QOS,MQTT_TOPIC1,MQTTPublish[BC26PubIndex].Data);
                pub_times++;
                
                break;
			}
			else
				BC26PubIndex++;
		case 2:
			if(MQTTPublish[BC26PubIndex].PublishOrNo==1){
			BC26TX.TXRequest(BC26PubResultHandle,"AT+QMTPUB=0,1,%d,0,\"%s/result/%s\",\"%d\"\r\n",BC26_QOS,MQTT_BT,ClientID,MQTTPublish[BC26PubIndex].Data);
			pub_times++;
                break;
			}
			else
				BC26PubIndex++;
		case 3:
			if(MQTTPublish[BC26PubIndex].PublishOrNo==1){
			BC26TX.TXRequest(BC26PubResultHandle,"AT+QMTPUB=0,1,%d,0,\"%s/result/%s\",\"%d\"\r\n",BC26_QOS,MQTT_ACTION,ClientID,MQTTPublish[BC26PubIndex].Data);
			pub_times++;
                break;
			}
			else
				BC26PubIndex++;
		case 4:
			if(MQTTPublish[BC26PubIndex].PublishOrNo==1){
			BC26TX.TXRequest(BC26PubResultHandle,"AT+QMTPUB=0,1,%d,0,\"%s/%s\",\"%d\"\r\n",BC26_QOS,MQTT_REPORT,ClientID,MQTTPublish[BC26PubIndex].Data);
			pub_times++;
                break;
			}
			else
				BC26PubIndex++;
		case 5:
			if(MQTTPublish[BC26PubIndex].PublishOrNo==1){
			BC26TX.TXRequest(BC26PubResultHandle,"AT+QMTPUB=0,1,%d,0,\"%s/result/%s\",\"%d\"\r\n",BC26_QOS,MQTT_HEART,ClientID,MQTTPublish[BC26PubIndex].Data);
			pub_times++;
                break;
			}
			else
				BC26PubIndex++;
		case 6:
			if(BC26ResetTask.Sleep==TASK_SLEEP){
				DEBUG("ALL Pub OK\r\n");
				BC26CheckTask.Sleep=TASK_NO_SLEEP;
			}
			else
				DEBUG("Pub TO RESET.\r\n");
			BC26PubIndex=0;
			TaskAddr->Sleep=TASK_SLEEP;
		break;
	}
}
