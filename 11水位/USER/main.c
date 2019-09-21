#include "stm32f10x.h"
#include "water.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
/*ˮλ����*/

int main ()
{
    uart_init(9600);
    water_init();
    delay_init();
    OLED_Init();
    while(1)
    {
        
        u8 * buf;
        buf=get_water_state(get_water_data());
        OLED_ShowString(0,0,buf,12);
        OLED_Refresh_Gram();
        printf("  ˮλ��%s\r\n",get_water_state(get_water_data()));
        Decide_Water_ERROR(get_water_data());
        printf("  0�쳣��1����  %d \r\n  ",Water_Condition);
        delay_ms(1000);
    }

}