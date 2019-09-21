#ifndef __BC26TX_H__
#define __BC26TX_H__

#include "stdint.h"
#include "Task.h"

#define BC26_TX_BUFFER_SIZE 129

#define BC26TX_BUFFER_EMPTY 0
#define BC26TX_BUFFER_NEW 1
#define BC26TX_BUFFER_ASKING 2
#define BC26TX_BUFFER_READY 3

#define BC26TX_REPEAT 0
#define BC26TX_FINISH 1
#define BC26TX_WAIT 2

typedef struct {
	char Buffer[BC26_TX_BUFFER_SIZE];
	uint8_t BuffersStatus;
	Task AtCommandTask;
	void (*TXTask)(Task *);
	void (*TXRequest)(uint8_t (*)(void),char *,...);
	uint8_t (*ATResultHandle)(void);
	void (*TXRepeat)(void);
	void (*TXFinish)(void);
}BC26TXTypeDef;

extern BC26TXTypeDef BC26TX;





#endif
