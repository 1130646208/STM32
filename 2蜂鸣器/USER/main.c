#include "stm32f10x.h"
#include "sys.h"
#include "delay.h"
#include "beep.h"



int main(void)
{
    delay_init();
    BEEP_Init();
    
    
    while(1)
    {
        BEEP=0;
        delay_ms(300);
        BEEP=1;
        delay_ms(300);
        
        
    }
    
    
    
}


