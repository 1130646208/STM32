#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "exti.h"
#include "usart.h"
int main(void)
{
    
    //定义
    
    //初始化
    delay_init();
    KEY_Init();
    LED_Init();
    EXTIX_Init();
    uart_init(115200);
    //功能
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    while(1)
    {
        printf("OK!\r\n");
        delay_ms(1000);
    } 

}



























