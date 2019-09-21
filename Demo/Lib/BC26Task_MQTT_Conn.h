#include "Task.h"
#ifndef __BC26TASK_MQTT_CONN_H__
#define __BC26TASK_MQTT_CONN_H__
#define BC26MQTT_CONN 1
#define BC26MQTT_NO_CONN 0

extern uint8_t BC26MQTTConned;
extern Task BC26MQTTConnTask;

void BC26MQTTConn(Task *TaskAddr);

#endif
