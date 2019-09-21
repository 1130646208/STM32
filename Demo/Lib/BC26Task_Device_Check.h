#include "Task.h"
#ifndef __BC26TASK_DEVICE_CHECK_H__
#define __BC26TASK_DEVICE_CHECK_H__
#define BC26CONNECT 1
#define BC26DISCONNECT 0

extern uint8_t BC26ConnectStatus;
extern Task BC26ConnectCheckTask;

void BC26ConnectCheck(Task *TaskAddr);

#endif
