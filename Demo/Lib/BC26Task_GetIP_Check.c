#include "BC26Task_GetIP_Check.h"
#include "BC26Task_Device_Check.h"
#include "BC26Task_MQTT_Init.h"
#include "BC26TX.h"
#include "BC26RX.h"
#include "BC26.h"
#include "Timer.h"
#include "DEBUG.h"

uint8_t BC26GetIP=BC26NO_GET_IP;
Task BC26GetIPCheckTask;

const uint8_t COMMAND_IP_CHECK[]={"AT+CGATT?\r\n"};
const uint8_t COMMAND_IP_ACT[]={"AT+CGATT=1\r\n"};
const uint8_t COMMAND_IP_DEACT[]={"AT+CGATT=0\r\n"};

void BC26GetIPCheck(Task *TaskAddr)
{
	static Timer CheckTimer;
	static uint8_t CommandAsk=BC26_COMMAND_READY_ASK;
	static uint8_t Index=0;
	static uint8_t FailTimes=0;
	uint16_t Times;
	uint8_t i,Row;
	if(BC26ConnectStatus==BC26CONNECT){
        //如果BC26连接上了
        
        //发送缓冲为空，并且准备好发送命令
		if((CommandAsk==BC26_COMMAND_READY_ASK)&&(BC26TxBuffersStatus==BC26TX_BUFFER_EMPTY)){
			if(Index==0){
				for(i=0;COMMAND_IP_CHECK[i]!='\0';i++)
					BC26TxBuffers[i]=COMMAND_IP_CHECK[i];
			}
			else if(Index==2){
				for(i=0;COMMAND_IP_DEACT[i]!='\0';i++)
					BC26TxBuffers[i]=COMMAND_IP_DEACT[i];
			}
			else{
				for(i=0;COMMAND_IP_ACT[i]!='\0';i++)
					BC26TxBuffers[i]=COMMAND_IP_ACT[i];
			}
			BC26TxBuffers[i]='\0';
			BC26TxBuffersStatus=BC26TX_BUFFER_NEW;
			CommandAsk=BC26_COMMAND_ASKING;
			TimerAdd(&CheckTimer,1000,1);
		}
        
        //正发送命令？
		else if(CommandAsk==BC26_COMMAND_ASKING){
            //发送缓冲区ready
			if(BC26TxBuffersStatus==BC26TX_BUFFER_READY){
				if((BC26RxATStartRow+1)==BC26RX_MAXROW)
					Row=0;
				else
					Row=BC26RxATStartRow+1;
				if(Index==0){
                    //发送的AT+CGATT?
					if(BC26RxBuffer[Row][BC26RxLength[Row]-2]=='0'){
                        //根据返回内容判断出没有得到IP
						BC26GetIP=BC26NO_GET_IP;
						Times=2000;
						Index=1;
						FailTimes++;
						DEBUG("NO GET IP.\r\n");
					}
                    //根据返回内容判断得到IP
					else if(BC26RxBuffer[Row][BC26RxLength[Row]-2]=='1'){
						
						Times=500;
						FailTimes=0;
						if(BC26GetIP==BC26NO_GET_IP){
							DEBUG("GET IP.\r\n");
							BC26MQTTInitTask.Sleep=TASK_NO_SLEEP;//得到IP地址之后，下一步 初始化MQTT
						}
						BC26GetIP=BC26GET_IP;//标志位
					}
				}
                
                //发送的AT+CGATT=1/0
				else{
					Times=2000;
					if(FailTimes>=10){
						FailTimes=0;
						Index=2;//失败10次则发送 AT+CGATT=0
					}
					else
						Index=0;//发送AT+CGATT?询问IP地址
				}
                
				BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
				TimerSub(&CheckTimer);
				CommandAsk=BC26_COMMAND_WAIT_ASK;
				TimerAdd(&CheckTimer,Times,1);//等待两秒
			}

            
			else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
				BC26ConnectStatus=BC26DISCONNECT;
				BC26GetIP=BC26NO_GET_IP;
				FailTimes=0;
				Index=0;
				BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
				CommandAsk=BC26_COMMAND_READY_ASK;
				TimerSub(&CheckTimer);
				TaskAddr->Sleep=TASK_SLEEP;
			}
		}
        
        //等待命令，超时
		else if((CommandAsk==BC26_COMMAND_WAIT_ASK)&&(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT)){
			TimerSub(&CheckTimer);
			CommandAsk=BC26_COMMAND_READY_ASK;//准备好重新发命令
		}
	}
    
    //若BC26未连接
	else{
		FailTimes=0;
		Index=0;
		TaskAddr->Sleep=TASK_SLEEP;//休眠任务
		BC26GetIP=BC26NO_GET_IP;
		CommandAsk=BC26_COMMAND_READY_ASK;
		if(CheckTimer.Using==TIMER_USING)
			TimerSub(&CheckTimer);
	}
}
