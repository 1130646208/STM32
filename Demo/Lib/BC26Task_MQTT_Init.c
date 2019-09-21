#include "BC26Task_MQTT_Init.h"
#include "BC26Task_GetIP_Check.h"
#include "BC26Task_Device_Check.h"
#include "BC26Task_MQTT_Open.h"
#include "BC26TX.h"
#include "BC26RX.h"
#include "BC26.h"
#include "DEBUG.h"
#include "Timer.h"

#define MQTT_KEEPALIVETIMES "10"		
#define MQTT_CLEANSESSION "0"		
#define MQTT_TIMEOUTTIMES "60"		
#define MQTT_TIMEOUTTRYTIMES "10"		
#define MQTT_USEWILL "0"		
#define MQTT_WILLQOS "0"		
#define MQTT_WILLRETAIN "1"		
#define MQTT_WILLTOPIC "\"test1/status\""		
#define MQTT_WILLMSG "\"IT DIE.\""			

#define MQTT_AT_QMTCFG_KEEPALIVE(X) "AT+QMTCFG=\"keepalive\",0,"X"\r\n"
#define MQTT_AT_QMTCFG_SESSION(X) "AT+QMTCFG=\"session\",0,"X"\r\n"
#define MQTT_AT_QMTCFG_TIMEOUT(X,Y) "AT+QMTCFG=\"timeout\",0,"X","Y",1\r\n"
#define MQTT_AT_QMTCFG_WILL(M,N,X,Y,Z) "AT+QMTCFG=\"will\",0,"M","N","X","Y","Z"\r\n"

const uint8_t COMMAND_MQTT_INIT_KEEPALIVE[]={MQTT_AT_QMTCFG_KEEPALIVE(MQTT_KEEPALIVETIMES)};
const uint8_t COMMAND_MQTT_INIT_SESSION[]={MQTT_AT_QMTCFG_SESSION(MQTT_CLEANSESSION)};
const uint8_t COMMAND_MQTT_INIT_TIMEOUT[]={MQTT_AT_QMTCFG_TIMEOUT(MQTT_TIMEOUTTIMES,MQTT_TIMEOUTTRYTIMES)};
const uint8_t COMMAND_MQTT_INIT_WILL[]={MQTT_AT_QMTCFG_WILL(MQTT_USEWILL,MQTT_WILLQOS,MQTT_WILLRETAIN,MQTT_WILLTOPIC,MQTT_WILLMSG)};

uint8_t BC26MQTTInited=BC26MQTT_NO_INIT;
Task BC26MQTTInitTask;

void BC26MQTTInit(Task *TaskAddr)
{
	static Timer CheckTimer;
	static uint8_t CommandAsk=BC26_COMMAND_READY_ASK;
	static uint8_t Index=0;
	uint8_t i,Row;
	if(BC26ConnectStatus==BC26CONNECT){
		if((CommandAsk==BC26_COMMAND_READY_ASK)&&(BC26TxBuffersStatus==BC26TX_BUFFER_EMPTY)){
			switch(Index){
				case 0:
								for(i=0;COMMAND_MQTT_INIT_KEEPALIVE[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_MQTT_INIT_KEEPALIVE[i];//保持连接 
								break;
				case 1:
								DEBUG("MQTT INIT 1,OK\r\n");
								for(i=0;COMMAND_MQTT_INIT_SESSION[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_MQTT_INIT_SESSION[i];//会话
								
								break;
				case 2:
								DEBUG("MQTT INIT 2,OK\r\n");
								for(i=0;COMMAND_MQTT_INIT_TIMEOUT[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_MQTT_INIT_TIMEOUT[i];//超时
								break;
				case 3:
								DEBUG("MQTT INIT 3,OK\r\n");
								for(i=0;COMMAND_MQTT_INIT_WILL[i]!='\0';i++)
									BC26TxBuffers[i]=COMMAND_MQTT_INIT_WILL[i];//遗嘱
								break;
				case 4:
								DEBUG("MQTT INIT 4,OK\r\n");
								BC26MQTTInited=BC26MQTT_INIT;
								if(CheckTimer.Using==TIMER_USING)
									TimerSub(&CheckTimer);
								CommandAsk=BC26_COMMAND_READY_ASK;
								BC26MQTTOpenTask.Sleep=TASK_NO_SLEEP;
								TaskAddr->Sleep=TASK_SLEEP;
								break;
			}
			if(Index!=4){
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
//				if((BC26RxATStartRow+1)==BC26RX_MAXROW){
//					if(BC26RxBuffer[0][0]=='O')
//						Index++;
//				}
//				else{
//					if(BC26RxBuffer[BC26RxATStartRow+1][0]=='O')
//						Index++;
//				}
				BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
				TimerSub(&CheckTimer);
				CommandAsk=BC26_COMMAND_WAIT_ASK;
				TimerAdd(&CheckTimer,1,1);
			}
			else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
				BC26ConnectStatus=BC26DISCONNECT;
				BC26MQTTInited=BC26MQTT_NO_INIT;
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
		BC26MQTTInited=BC26MQTT_NO_INIT;
		Index=0;
		CommandAsk=BC26_COMMAND_READY_ASK;
		if(CheckTimer.Using==TIMER_USING)
			TimerSub(&CheckTimer);
	}
}
