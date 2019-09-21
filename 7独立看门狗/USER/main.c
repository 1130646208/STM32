#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "wdg.h"


int main()
{
    delay_init();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    uart_init(115200);
    LED_Init();
    KEY_Init();
    delay_ms(500);//Tout=((4°¡2^prer) °¡rlr) /40
    IWDG_Init(4,625);
    
    LED0=0;//µ„¡¡
    
    while(1)
    {
        if(KEY_Scan(0)==WKUP_PRES)
        {
            IWDG_Feed(); //Œππ∑
            
        }
        
        delay_ms(10);
        
    };
     
}