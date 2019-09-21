#include "BC26.h"
#include "BC26RX.h"
#include "BC26TX.h"
#include "Delay.h"
#include "CircularQueue.h"
#include "BC26Task_Device_Check.h"
#include "BC26Task_GetIP_Check.h"
#include "BC26Task_MQTT_Init.h"
#include "BC26Task_MQTT_Open.h"
#include "BC26Task_MQTT_Conn.h"
#include "BC26TASK_MQTT_Sub.h"
#include "BC26TASK_MQTT_Unsub.h"
#include "BC26TASK_MQTT_Pub.h"

CircularQueue *BC26StartTask=CIRCULARQUEUE_NULL;

void BC26TaskInit(void)
{
//(Task* TaskAddr,CircularQueue** StartTask,void (*Doing)(Task*),uint8_t Sleep)
//	TaskAdd(&BC26ConnectCheckTask,&BC26StartTask,BC26ConnectCheck,TASK_NO_SLEEP);
//	TaskAdd(&BC26GetIPCheckTask,&BC26StartTask,BC26GetIPCheck,TASK_SLEEP);
//	TaskAdd(&BC26MQTTInitTask,&BC26StartTask,BC26MQTTInit,TASK_SLEEP);
//	TaskAdd(&BC26MQTTOpenTask,&BC26StartTask,BC26MQTTOpen,TASK_SLEEP);
//	TaskAdd(&BC26MQTTConnTask,&BC26StartTask,BC26MQTTConn,TASK_SLEEP);
//	
//	//TaskAdd(&BC26MQTTSubTask,&BC26StartTask,BC26MQTTSub,TASK_SLEEP);
//	TaskAdd(&BC26MQTTSPubTask,&BC26StartTask,BC26MQTTPub,TASK_SLEEP);
//	//TaskAdd(&BC26ResponseCheckTask,&BC26StartTask,BC26ResponseCheck,TASK_NO_SLEEP);
//	//TaskAdd(&BC26MQTTUnsubTask,&BC26StartTask,BC26MQTTUnsub,TASK_SLEEP);
//	
//	TaskAdd(&BC26TxAtCommandTask,&BC26StartTask,BC26TxAtCommand,TASK_NO_SLEEP);
    BC26Connect();
    //TaskAdd(&BC26ConnectCheckTask,&BC26StartTask,BC26ConnectCheck,TASK_NO_SLEEP);
    TaskAdd(&BC26GetIPCheckTask,&BC26StartTask,BC26GetIPCheck,TASK_NO_SLEEP);
    TaskAdd(&BC26MQTTInitTask,&BC26StartTask,BC26MQTTInit,TASK_SLEEP);
    TaskAdd(&BC26MQTTOpenTask,&BC26StartTask,BC26MQTTOpen,TASK_SLEEP);
    TaskAdd(&BC26MQTTConnTask,&BC26StartTask,BC26MQTTConn,TASK_SLEEP);
    
    
    TaskAdd(&BC26TxAtCommandTask,&BC26StartTask,BC26TxAtCommand,TASK_NO_SLEEP);
    
    
}
void BC26GpioInit()
{
	GPIO_InitTypeDef GPIOInit;
	RCC_APB2PeriphClockCmd(BC26_RESET_PORT_RCC,ENABLE);
	GPIOInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIOInit.GPIO_Pin=BC26_RESET_PIN;
	GPIO_Init(BC26_RESET_PORT,&GPIOInit);
	
	GPIO_SetBits(BC26_RESET_PORT,BC26_RESET_PIN);

}
void BC26Reset()
{
	GPIO_SetBits(BC26_RESET_PORT,BC26_RESET_PIN);
	DelayMs(1500);
	GPIO_ResetBits(BC26_RESET_PORT,BC26_RESET_PIN);
	DelayMs(500);
}
void BC26Connect()
{
    BC26ConnectStatus=BC26CONNECT;
}