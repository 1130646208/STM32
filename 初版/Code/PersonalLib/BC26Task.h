#ifndef __BC26TASK_H__
#define __BC26TASK_H__

#include "stm32f10x.h"
#include "BC26RX.h"
#include "BC26TX.h"
#include "BC26.h"
#include "Task.h"
#include "Usart.h"
#include "Timer.h"
#include "DEBUG.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "Lock.h"
#include "flash.h"

#define BC26_QOS 2
#define MQTT_ACTIVATE "activate"
#define MQTT_STATUS "UseStatus"
#define MQTT_TOPIC1 "hello"

#define MQTT_BT "blueCode"
#define MQTT_ACTION "open"
#define MQTT_QUERY "query"
#define MQTT_HEART "heart"
#define MQTT_REPORT "report"

#define MAX_ERROR_TIMES 10  //

extern char StringTemp[128];

extern Task BC26CheckTask;
void BC26Check(Task *TaskAddr);

extern Task BC26PSMSetTask;
void BC26PSMSet(Task *TaskAddr);

extern Task BC26CardCheckTask;
void BC26CardCheck(Task *TaskAddr);

extern Task BC26IPCheckTask;
void BC26IPCheck(Task *TaskAddr);

extern Task BC26MQTTInitTask;
void BC26MQTTInitCheck(Task *TaskAddr);

extern uint8_t BC26SuccessOpen;
extern Task BC26OpenTask;
void BC26Open(Task *TaskAddr);
extern char IPAddr[16];
extern char Port[6];

extern uint8_t BC26SuccessConnect;
extern Task BC26ConnectTask;
void BC26Connect(Task *TaskAddr);
extern char Password[17];
extern char UserName[17];

extern uint8_t BC26Subed;
extern uint8_t BC26SubOK;
extern Task BC26SubTask;
void BC26Sub(Task *TaskAddr);

extern uint8_t BC26Unsubed;
extern uint8_t BC26UnsubOK;
extern Task BC26UnsubTask;
void BC26Unsub(Task *TaskAddr);

/*************************************/
typedef struct{
	uint8_t PublishOrNo;
	char Data[30];
}MQTTPublishTypedef;


extern MQTTPublishTypedef MQTTPublish[6];
extern uint8_t BC26CanPub;
extern uint8_t BC26PubOK;
extern Task BC26PubTask;
void BC26Pub(Task *TaskAddr);


extern uint8_t pub_times;

#endif
