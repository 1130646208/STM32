#include "Task.h"
#ifndef __BC26TASK_MQTT_INIT_H__
#define __BC26TASK_MQTT_INIT_H__
#define BC26MQTT_INIT 1
#define BC26MQTT_NO_INIT 0

extern uint8_t BC26MQTTInited;
extern Task BC26MQTTInitTask;

void BC26MQTTInit(Task *TaskAddr);

#endif
