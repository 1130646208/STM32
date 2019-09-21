#include "stm32f10x.h"
#include "esp8266.h"
#include "delay.h" 
#include "test.h"
int main(void)
{	
    
    
    delay_init();
    TIMER6_Init(7199,9999);//每隔1s进入一次中断
    OLED_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    ESP8266_Init(9600);//使用前一定要注意，模块波特率与单片机匹配！！！
    
    while(1){

        ESP8266_Send_Data("12345678901",11);
        delay_ms(500);
       
    }
}
