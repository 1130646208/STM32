#include "Lock.h"
#include "DEBUG.h"
#include "Task.h"
#include "Timer.h"

char ClientID[17]="ClientID";
char Status[17]="2";
char HeartTimes[17]="1";
char AuthCode[17]="000000";
char BTPassword[17]="123456";
uint8_t IDChanged=0;

Task LockOpenCloedTask;
Timer LockOpenCloedTimer={.Using=TIMER_NO_USING};

void LockGpioInit(void)
{
	GPIO_InitTypeDef GPIOInit;
	RCC_APB2PeriphClockCmd(LOCK_RCC,ENABLE);
	GPIOInit.GPIO_Mode=GPIO_Mode_IPU;
	GPIOInit.GPIO_Pin=LOCK_DOOR_CHECK_PIN|LOCK_CHECK_PIN;
	GPIO_Init(LOCK_PORT,&GPIOInit);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInit.GPIO_Pin=LOCK_PIN_1|LOCK_PIN_2;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(LOCK_PORT,&GPIOInit);
	
	//LockChange(LOCK_CLOSE);	
}
void LockOpenCloed(Task *TaskAddr)
{
	
}
