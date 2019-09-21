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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//��ʼ�����ж�
	DelayInit();		//��ʱ������ʼ��
	DelayMs(1000);		//��ʱ1s���ȴ���Χ��·׼������
	Usart1Init();		//��ʼ������1
	Usart3Init();		//��ʼ������3
	TimeInit();		//��ʱ����ʼ��
	GETDevicesID();
	BC26GpioInit();		//��ʼ��BT26�������
    BC26Connect();
	DEBUG("System Init OK.\r\n");
}

