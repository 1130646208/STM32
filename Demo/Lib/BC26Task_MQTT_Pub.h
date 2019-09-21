#include "Task.h"
#ifndef __BC26TASK_MQTT_PUB_H__
#define __BC26TASK_MQTT_PUB_H__
#define BC26MQTT_PUB 1
#define BC26MQTT_NO_PUB 0

#define MQTT_PUB_ACTIVATE "activate/result/"
#define MQTT_PUB_STATUS "useStatus/result/"
#define MQTT_PUB_BT "blueCode/result/"
#define MQTT_PUB_ACTION "open/result/"
#define MQTT_PUB_HEART "heart/result/"
#define MQTT_PUB_REPORT "report/"

#define MQTT_PUB_ACTIVATE_INDEX 0
#define MQTT_PUB_STATUS_INDEX 1
#define MQTT_PUB_BT_INDEX 2
#define MQTT_PUB_ACTION_INDEX 3
#define MQTT_PUB_REPORT_INDEX 4
#define MQTT_PUB_HEART_INDEX 5

#define MQTT_PUB_MAX 6

extern Task BC26MQTTSPubTask;
extern uint8_t PublicQueue[MQTT_PUB_MAX];
extern uint8_t PublicQueueData[MQTT_PUB_MAX];

void BC26MQTTPub(Task *TaskAddr);

#endif
