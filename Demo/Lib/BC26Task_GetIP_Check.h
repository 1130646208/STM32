#include "Task.h"
#ifndef __BC26TASK_GETIP_CHECK_H__
#define __BC26TASK_GETIP_CHECK_H__
#define BC26GET_IP 1
#define BC26NO_GET_IP 0

extern uint8_t BC26GetIP;
extern Task BC26GetIPCheckTask;

extern const uint8_t COMMAND_IP_ACT[];
extern const uint8_t COMMAND_IP_DEACT[];

void BC26GetIPCheck(Task *TaskAddr);

#endif
