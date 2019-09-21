#include "stm32f10x.h"
#include "hx711.h"
#include "delay.h"
#include "usart.h"
int main()
{
    
    uart_init(9600);
    hx711_init();
    delay_init();
    hx711_calib();
    while(1){
    
        printf("Ñ¹Á¦£º%s\n",get_hx711_status(get_hx711_data()-hx711_init_val));
        delay_ms(500);
    }
        
    
}