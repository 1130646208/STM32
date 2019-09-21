#include "stm32f10x.h"
#include "smoke.h"
#include "delay.h"
#include "usart.h"
#include "oled.h"
#include "myiic.h"
int main(void)
{
    
    delay_init();
    mq4_init();
    uart_init(9600);
    IIC_Init();
    OLED_Init();
    
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    
    
    while(1)
    {
//        const u8 * temp;
        printf("value:%f\n",get_co_value());//串口调试
 //      temp=(const u8 *)get_real_data(gas_level_status());
        
//        OLED_ShowString(0,0,"CONCENTRATION",12);////size:选择字体 12/16/24
//        OLED_ShowString(0,12,"_____________",12);
//        SMOKE_Display(temp);
//        OLED_Refresh_Gram();

        
        delay_ms(1000);
    }

}
