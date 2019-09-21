#ifndef __BC26RX_H__
#define __BC26RX_H__

#include "stm32f10x.h"
#include "Task.h"

#define BC26_RX_BUFFER_TEMP_SIZE 128
#define BC26_RX_ABUFFER_SIZE 128
#define BC26_RX_PBUFFER_SIZE 256

#define BC26_ATCOM_ASK 0
#define BC26_ATCOM_ANALYSIS 1
#define BC26_ATCOM_RESPOND 2

#define BC26_ATCOM_ERROR 0
#define BC26_ATCOM_OK 1

typedef struct {
	void (*HardWareInit)(void);
	char BufferTemp[BC26_RX_BUFFER_TEMP_SIZE];
	char ABuffer[BC26_RX_ABUFFER_SIZE];
	char PBuffer[BC26_RX_PBUFFER_SIZE];
	uint16_t PSegment;
	uint8_t ATCOMStatus;
	uint8_t ATCOMResult;
	uint16_t PBufferReadPoint;
	Task BC26RXPBufferReaderTask;
	void (*BC26RXPBufferReader)(Task *TaskAddr);
}BC26RXTypeDef;

extern BC26RXTypeDef BC26RX;

#endif
