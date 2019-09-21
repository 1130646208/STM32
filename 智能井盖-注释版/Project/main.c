#include "SystemInit.h"
/*
    功能：数函数
    返回值：无
*/
int main()
{
	CircularQueue *TaskTemp;		
	SystemHardwareInit();	
    //循环执行任务队列
	while(1){
		RunTaskQueue(&SystemStartTask,&TaskTemp);	   
	}   
}
