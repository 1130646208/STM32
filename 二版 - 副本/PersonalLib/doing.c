#include "SystemInit.h"
extern uint8_t BC26SubIndex;
uint8_t BC26SubResultHandle(void);
extern uint8_t BC26UnsubIndex;
uint8_t BC26UnsubResultHandle(void);
extern uint8_t BC26PubIndex;
uint8_t BC26PubResultHandle(void);
MQTTPublishTypedef MQTTPublish[MAX_PUB_TOPIC]={{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0}};
char IPAddr[16]={"47.106.249.19"};//
char Port[6]={"61613"};//
char Password[17]={"password"};//password
char UserName[17]={"admin"};//admin
char ID[10];
char TOPIC[20]={"state/"};
uint8_t BC26CanPub=1;	
uint8_t BC26Unsubed=1;
uint8_t BC26Subed=0;
/**********************/
uint8_t PUB_Times=0;
int counter=0;
Task test;
extern char* receiv;


void BC26Sub(Task *TaskAddr)
{
	if(BC26Subed==1)		
		BC26SubIndex=0;
	switch(BC26SubIndex){
		case 1:
			BC26TX.TXRequest(BC26SubResultHandle,"AT+QMTSUB=0,1,\"%s\",%d\r\n",TOPIC,BC26_QOS);
            //DEBUG("!!!!");
		break;
		default:
			if(BC26ResetTask.Sleep==TASK_SLEEP){
				if(BC26Subed==0)
					DEBUG("ALL Sub OK\r\n");
				BC26Subed=1;
				WakeUpNextTask(TaskAddr);
			}
			else
				DEBUG("Sub TO RESET.\r\n");
			BC26SubIndex=1;
			TaskAddr->Sleep=TASK_SLEEP;
		break;
	}
}
void BC26Unsub(Task *TaskAddr)
{
//    WakeUpNextTask(TaskAddr);
//    TaskAddr->Sleep=TASK_SLEEP;
//    return;
	if((BC26Unsubed==1)||(BC26Subed==0))	
		BC26UnsubIndex=0;
	switch(BC26UnsubIndex){
		case 1:
			BC26TX.TXRequest(BC26UnsubResultHandle,"AT+QMTUNS=0,1,\"%s\"\r\n",TOPIC);
            DEBUG("\r\n");DEBUG("Unsub.");DEBUG("\r\n");
		break;
		default:
			if(BC26ResetTask.Sleep==TASK_SLEEP){
				if((BC26Unsubed==0)&&(BC26Subed==1))
					BC26Unsubed=1;
				WakeUpNextTask(TaskAddr);
			}
			else{
				DEBUG("Unsub TO RESET.\r\n");
			}
			BC26UnsubIndex=1;
			TaskAddr->Sleep=TASK_SLEEP;
		break;
	}
}
void BC26Pub(Task *TaskAddr)
{
	uint8_t i;
	if((BC26CanPub==0)||(BC26ResetTask.Sleep==TASK_NO_SLEEP))
		BC26PubIndex=0;
	else if(BC26NeedPub==0){		
		for(i=0;i<MAX_PUB_TOPIC;i++){
			if(MQTTPublish[i].PublishOrNo==1){
				BC26NeedPub=1;
				BC26PubIndex=i+1;
				break;
			}
			else{
				BC26PubIndex=0;	
				BC26NeedPub=0;
			}
		}
	}
	switch(BC26PubIndex){
		case 1:
			BC26TX.TXRequest(BC26PubResultHandle,"AT+QMTPUB=0,1,%d,0,\"%s\",\"%s\"\r\n",BC26_QOS,TOPIC,MQTTPublish[BC26PubIndex-1].DataString);
            PUB_Times++;
        break;
		default:
			if(BC26ResetTask.Sleep==TASK_SLEEP){
				BC26CheckTask.Sleep=TASK_NO_SLEEP;		
			}
			else
				DEBUG("Pub To RESET.\r\n");
			BC26NeedPub=0;
			BC26PubIndex=1;
			TaskAddr->Sleep=TASK_SLEEP;
		break;
	}
}
void UserTaskInit(void)
{
    TaskAdd(&test,&SystemStartTask,test_func,TASK_NO_SLEEP);
	
}
void UserHardwareInit(void)
{
	all_sensor_init();
    Usart2Init();
//    while(1){
//        if(SetNetInfo())
//            break;
//    }
    setTPC();
    FirstRunning();
    InitDevicesDatas();
    start_pub();
    
}
void MQTTRXHandle(char *String)		
{
    DEBUG("Receive sub..........");
	if(sscanf(StringTemp,"+QMTRECV: 0,%*["Z2N"],\"%127[^\"]\",\"%127[^\"]\"",StringTemp1,StringTemp2)==2){
    DEBUG(StringTemp2);DEBUG("!!!");
	}
}
