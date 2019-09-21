#include "Task.h"
#ifndef __BC26TASK_MQTT_SUB_H__
#define __BC26TASK_MQTT_SUB_H__
#define BC26MQTT_SUB 1
#define BC26MQTT_NO_SUB 0

#define MQTT_SUB_ACTIVATE "activate/"
#define MQTT_SUB_STATUS "useStatus/"
#define MQTT_SUB_BT "blueCode/"
#define MQTT_SUB_ACTION "open/"
#define MQTT_SUB_QUERY "query/"
#define MQTT_SUB_HEART "heart/"

extern uint8_t BC26MQTTSubed;
extern Task BC26MQTTSubTask;

void BC26MQTTSub(Task *TaskAddr);

#endif
