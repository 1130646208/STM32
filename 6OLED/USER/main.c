#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "oled.h"
#include "myiic.h"
#include "stm32f10x.h"



int main(void)
{					      	

    
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    LED_Init();
    IIC_Init();
    OLED_Init();
    
    
    OLED_ShowString(0,0,"CONCENTRATION",12);////size:Ñ¡Ôñ×ÖÌå 12/16/24
    OLED_ShowString(0,24,"ERROR",24);
    OLED_ShowString(64,24,"ppm",24);
    OLED_Refresh_Gram();
    

    while(1){}
}
