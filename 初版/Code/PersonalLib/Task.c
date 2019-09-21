#include "Task.h"

void TaskAdd(Task* TaskAddr,CircularQueue** StartTask,void (*Doing)(Task*),uint8_t Sleep)
{
	TaskAddr->Sleep=Sleep;
	TaskAddr->Doing=Doing;
	TaskAddr->TaskQueue.Data=TaskAddr;
	TaskAddr->TaskQueue.StartAddr=StartTask;
	QueueAdd(&(TaskAddr->TaskQueue));
}
void TaskSub(Task* TaskAddr)
{
	QueueSub(&(TaskAddr->TaskQueue));
	TaskAddr->Sleep=TASK_SLEEP;
}
void RunTaskQueue(CircularQueue** StartTask,CircularQueue** TaskTemp)
{
	if((*StartTask)!=TASK_NULL){		
		if((*TaskTemp)==TASK_NULL)			
			(*TaskTemp)=(*StartTask);		
		if(((Task*)((*TaskTemp)->Data))->Sleep==TASK_NO_SLEEP)	
			((Task*)((*TaskTemp)->Data))->Doing(((Task*)((*TaskTemp)->Data)));	
		(*TaskTemp)=(*TaskTemp)->NextOne;		
	}
	else {	
		if((*TaskTemp)!=TASK_NULL)
			(*TaskTemp)=TASK_NULL;
	}
}
void WakeUpNextTask(Task *TaskAddr)
{
	if((TaskAddr->TaskQueue.NextOne)!=(*(TaskAddr->TaskQueue.StartAddr)))
		((Task*)(TaskAddr->TaskQueue.NextOne->Data))->Sleep=TASK_NO_SLEEP;
}
