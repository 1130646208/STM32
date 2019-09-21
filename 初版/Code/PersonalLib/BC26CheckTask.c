#include "BC26Task.h"

Task BC26CheckTask;
uint8_t BC26CheckResult=0;

uint8_t BC26CheckResultHandle(void);

uint8_t BC26CheckResultHandle(void)
{
	DEBUG("BC26 exist\r\n");
	BC26CheckResult=1;
	return BC26TX_FINISH;
}
void BC26Check(Task *TaskAddr)
{
	if(BC26CheckResult==0)
		BC26TX.TXRequest(BC26CheckResultHandle,"ATI\r\n");
	else{
		BC26CheckResult=0;
		TaskAddr->Sleep=TASK_SLEEP;
		WakeUpNextTask(TaskAddr);
	}
}