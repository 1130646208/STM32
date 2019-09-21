#include "stm32f10x.h"
#include "water.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
/*水位测试*/

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
        printf("  水位：%s\r\n",get_water_state(get_water_data()));
        Decide_Water_ERROR(get_water_data());
        printf("  0异常，1正常  %d \r\n  ",Water_Condition);
        delay_ms(1000);
    }

}