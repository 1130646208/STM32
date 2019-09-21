#include "BC26Task.h"

Task BC26PSMSetTask;
uint8_t BC26PSMSetResult=0;

uint8_t BC26PSMSetResultHandle(void);

uint8_t BC26PSMSetResultHandle(void)
{
	DEBUG("PSM Mode Set\r\n");
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		BC26PSMSetResult=1;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
void BC26PSMSet(Task *TaskAddr)
{
	if(BC26PSMSetResult==0)
		BC26TX.TXRequest(BC26PSMSetResultHandle,"AT+CPSMS=0\r\n");
	else{
		BC26PSMSetResult=0;
		TaskAddr->Sleep=TASK_SLEEP;
		WakeUpNextTask(TaskAddr);
	}
}
