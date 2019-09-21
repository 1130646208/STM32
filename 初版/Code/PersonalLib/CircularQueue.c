#include "CircularQueue.h"

void QueueAdd(CircularQueue* CircularQueueAddr)//向队列中添加任务
{
	if(CircularQueueAddr->IsAdded==CIRCULARQUEUE_NO_ADDED){
		if((*(CircularQueueAddr->StartAddr))==CIRCULARQUEUE_NULL){//如果当前队列为空
			(*(CircularQueueAddr->StartAddr))=CircularQueueAddr;//起始任务地址为该任务地址
			CircularQueueAddr->NextOne=CircularQueueAddr;//下一个任务为它本身
			CircularQueueAddr->PreviousOne=CircularQueueAddr;//前一个任务为它本身
		}
		else{//队列不为空
			CircularQueueAddr->PreviousOne=(*(CircularQueueAddr->StartAddr))->PreviousOne;//该任务的前一个任务为起始任务的前一个任务
			CircularQueueAddr->NextOne=(*(CircularQueueAddr->StartAddr));//该任务的下一个任务为该任务
			(*(CircularQueueAddr->StartAddr))->PreviousOne->NextOne=CircularQueueAddr;//起始任务的前一个任务的下一个任务设置为该任务
			(*(CircularQueueAddr->StartAddr))->PreviousOne=CircularQueueAddr;//起始任务的前一个任务为该任务
            
		}
		CircularQueueAddr->IsAdded=CIRCULARQUEUE_ADDED;
	}
}
void QueueSub(CircularQueue* CircularQueueAddr)//删除队列中任务
{
	if(CircularQueueAddr->IsAdded==CIRCULARQUEUE_ADDED){
		if((CircularQueueAddr->NextOne)==CircularQueueAddr)//如果只有一个任务
			(*(CircularQueueAddr->StartAddr))=CIRCULARQUEUE_NULL;//设置该任务队列为空
		else{
			CircularQueueAddr->PreviousOne->NextOne=CircularQueueAddr->NextOne;//该任务的前一个任务的下一个任务为该任务的下一个任务
			CircularQueueAddr->NextOne->PreviousOne=CircularQueueAddr->PreviousOne;//该任务的下一个任务的前一个任务为该任务的前一个任务
		}
		if(CircularQueueAddr==(*(CircularQueueAddr->StartAddr)))//如果删除的任务是头任务
			(*(CircularQueueAddr->StartAddr))=CircularQueueAddr->NextOne;//将该任务的下一个任务设置为头
		CircularQueueAddr->IsAdded=CIRCULARQUEUE_NO_ADDED;
	}
}
