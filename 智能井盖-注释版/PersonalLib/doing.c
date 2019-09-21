#include "SystemInit.h"
extern uint8_t BC26SubIndex;
uint8_t BC26SubResultHandle(void);
extern uint8_t BC26UnsubIndex;
uint8_t BC26UnsubResultHandle(void);
extern uint8_t BC26PubIndex;
uint8_t BC26PubResultHandle(void);
MQTTPublishTypedef MQTTPublish[MAX_PUB_TOPIC]={{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0},{.PublishOrNo=0}};
char IPAddr[16]={"47.106.249.19"};//IP地址
char Port[6]={"61613"};//端口号
char Password[17]={"password"};//MQTT服务密码
char UserName[17]={"admin"};//MQTT服务账号
char ID[10];//井盖ID
char TOPIC[20]={"report/"};//发布Topic
/*一些标志位*/
uint8_t BC26CanPub=1;//可以发布消息
uint8_t BC26Unsubed=1;//已经取消订阅
uint8_t BC26Subed=0;//已经订阅
/*一些变量*/
uint8_t PUB_Times=0;//发布次数
int counter=0;//定时器中断进入次数
Task test;//新建任务
extern char* receiv;//接收到消息

/*
    功能：订阅主题
    返回值：无
*/

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
/*
    功能：取消订阅主题
    返回值：无
*/
void BC26Unsub(Task *TaskAddr)
{

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
/*
    功能：发布消息到主题
    返回值：无
*/
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
/*
    功能：初始化用户任务
    返回值：无
*/
void UserTaskInit(void)
{
    TaskAdd(&test,&SystemStartTask,test_func,TASK_NO_SLEEP);
	
}
/*
    功能：用户硬件初始化
    返回值：无
*/
void UserHardwareInit(void)
{
	all_sensor_init();
    Usart2Init();
    while(1){
        if(SetNetInfo())
            break;
    }
    setTPC();
    FirstRunning();
    InitDevicesDatas();
    start_pub();
    
}
/*
    功能：BC26接收到消息，进入此函数并在此处理
    返回值：无
*/
void MQTTRXHandle(char *String)		
{
    DEBUG0("Receive sub.");
	if(sscanf(StringTemp,"+QMTRECV: 0,%*["Z2N"],\"%127[^\"]\",\"%127[^\"]\"",StringTemp1,StringTemp2)==2){
    DEBUG0(StringTemp2);DEBUG0("!!!");
	}
}
