#include "CircularQueue.h"
#include "stdint.h"
#ifndef __TASK_H__
#define __TASK_H__

#define TASK_NULL 0
#define TASK_SLEEP 1
#define TASK_NO_SLEEP 0

typedef struct TaskStruct{
	CircularQueue TaskQueue;
	uint8_t Sleep;
	void (*Doing)(struct TaskStruct *);
}Task;

void TaskAdd(Task* TaskAddr,CircularQueue** StartTask,void (*Doing)(Task*),uint8_t Sleep);
void TaskSub(Task* TaskAddr);
void RunTaskQueue(CircularQueue** StartTask,CircularQueue** TaskTemp);

#endif
