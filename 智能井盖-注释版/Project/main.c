#include "SystemInit.h"
/*
    ���ܣ�������
    ����ֵ����
*/
int main()
{
	CircularQueue *TaskTemp;		
	SystemHardwareInit();	
    //ѭ��ִ���������
	while(1){
		RunTaskQueue(&SystemStartTask,&TaskTemp);	   
	}   
}
