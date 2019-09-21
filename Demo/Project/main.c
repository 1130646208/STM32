#include "Usart.h"
#include "Delay.h"
#include "misc.h"
#include "Timer.h"
#include "Task.h"
#include "Debug.h"
#include "flash.h"
#include "BC26.h"

void InitTheSystem(void);

uint8_t Reset_Flag=0;

int main()
{
	CircularQueue *TaskTemp;
	InitTheSystem();
	DEBUG("System Start.\r\n");
	BC26TaskInit();
	while(1){
		RunTaskQueue(&BC26StartTask,&TaskTemp);
	}
	return 0;
}
void InitTheSystem()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//初始化终中断
	DelayInit();		//延时函数初始化
	DelayMs(1000);		//延时1s，等待外围电路准备就绪
	Usart1Init();		//初始化串口1
	Usart3Init();		//初始化串口3
	TimeInit();		//定时器初始化
	GETDevicesID();
	BC26GpioInit();		//初始化BT26相关引脚
    BC26Connect();
	DEBUG("System Init OK.\r\n");
}

