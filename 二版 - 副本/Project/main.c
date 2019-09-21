#include "SystemInit.h"

int main()
{
	CircularQueue *TaskTemp;		
	SystemHardwareInit();	

	while(1){
		RunTaskQueue(&SystemStartTask,&TaskTemp);	
        
	}
}

