#include "stm32f10x.h"
#include "esp8266.h"
#include "delay.h" 
#include "test.h"
int main(void)
{	
    
    
    delay_init();
    TIMER6_Init(7199,9999);//ÿ��1s����һ���ж�
    OLED_Init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    ESP8266_Init(9600);//ʹ��ǰһ��Ҫע�⣬ģ�鲨�����뵥Ƭ��ƥ�䣡����
    
    while(1){

        ESP8266_Send_Data("12345678901",11);
        delay_ms(500);
       
    }
}
