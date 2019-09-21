#include "stdint.h"
#ifndef __CIRCULARQUEUE_H__
#define __CIRCULARQUEUE_H__

#define CIRCULARQUEUE_NULL 0
#define CIRCULARQUEUE_NO_ADDED 0
#define CIRCULARQUEUE_ADDED 1
#define CIRCULARQUEUE_SUCCESS 1
#define CIRCULARQUEUE_FAIL 0

typedef struct CircularQueueStruct{
	struct CircularQueueStruct* PreviousOne;
	struct CircularQueueStruct* NextOne;
	struct CircularQueueStruct** StartAddr;
	uint8_t IsAdded;
	void* Data;
}CircularQueue;

void QueueAdd(CircularQueue* CircularQueueAddr);
void QueueSub(CircularQueue* CircularQueueAddr);

#endif
