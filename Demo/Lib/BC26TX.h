#include "stdint.h"
#include "Task.h"
#ifndef __BC26TX_H__
#define __BC26TX_H__
#define BC26TX_BUFFER_EMPTY 0
#define BC26TX_BUFFER_NEW 1
#define BC26TX_BUFFER_ASKING 2
#define BC26TX_BUFFER_READY 3

extern uint8_t BC26TxBuffers[129];
extern uint8_t BC26TxBuffersStatus;
extern Task BC26TxAtCommandTask;

void BC26TxAtCommand(Task *TaskAddr);

#endif
