#include "SystemInit.h"

void SystemTaskInit(void);
void TestLed(void)
{
	GPIO_InitTypeDef GPIOInit;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIOInit.GPIO_Pin=GPIO_Pin_13;
	GPIOInit.GPIO_Mode=GPIO_Mode_Out_PP;   
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIOInit);

 	GPIO_ResetBits(GPIOC,GPIO_Pin_13); 
}
int main()
{
	CircularQueue *TaskTemp;
    all_sensor_init();
//    while(1){
//        sen_test();
//        DelayMs(1000);
//    }
    SystemHardwareInit();
	while(1){
		RunTaskQueue(&SystemStartTask,&TaskTemp);
        
	}
}

