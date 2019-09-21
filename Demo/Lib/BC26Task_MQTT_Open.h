#include "Task.h"
#ifndef __BC26TASK_MQTT_OPEN_H__
#define __BC26TASK_MQTT_OPEN_H__
#define BC26MQTT_OPEN 1
#define BC26MQTT_NO_OPEN 0

extern uint8_t BC26MQTTOpened;
extern Task BC26MQTTOpenTask;

void BC26MQTTOpen(Task *TaskAddr);

#endif
