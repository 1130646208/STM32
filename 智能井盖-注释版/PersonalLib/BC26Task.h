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

#define BC26_QOS 2

#define MAX_ERROR_TIMES 10

extern char StringTemp[128];
extern char DevicesID[25];

extern Task BC26CheckTask;
void BC26Check(Task *TaskAddr);

extern Task BC26PSMSetTask;
void BC26PSMSet(Task *TaskAddr);

extern Task BC26CESQCheckTask;
void BC26CESQCheck(Task *TaskAddr);

extern Task BC26CardCheckTask;
void BC26CardCheck(Task *TaskAddr);

extern Task BC26IPCheckTask;
void BC26IPCheck(Task *TaskAddr);

extern Task BC26eDRXSetTask;
void BC26eDRXSet(Task *TaskAddr);

extern Task BC26REGSetTask;
void BC26REGSet(Task *TaskAddr);

extern Task BC26CSCONSetTask;
void BC26CSCONSet(Task *TaskAddr);

extern uint8_t MQTTNeedInit;
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
extern char ID[10];
extern char TOPIC[20];

extern uint8_t BC26Subed;
extern uint8_t BC26SubOK;
extern Task BC26SubTask;
void BC26Sub(Task *TaskAddr);

extern uint8_t BC26Unsubed;
extern uint8_t BC26UnsubOK;
extern Task BC26UnsubTask;
void BC26Unsub(Task *TaskAddr);

#define MAX_PUB_TOPIC 6
typedef struct{
	uint8_t PublishOrNo;
	int32_t DataInt;
	float DataFloat;
	char *DataString;
    char DataArray[30];
}MQTTPublishTypedef;
extern MQTTPublishTypedef MQTTPublish[MAX_PUB_TOPIC];
extern uint8_t BC26CanPub;
extern uint8_t BC26PubOK;
extern Task BC26PubTask;
extern uint8_t BC26NeedPub;
void BC26Pub(Task *TaskAddr);

extern Task BC26DetachTask;
void BC26Detach(Task *TaskAddr);

#endif
