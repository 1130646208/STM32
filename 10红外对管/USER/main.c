#include "stm32f10x.h"
#include "infrared.h"
#include "oled.h"
#include "myiic.h"
#include "delay.h"
#include "usart.h"

int main()
{
    delay_init();
    IIC_Init();
    OLED_Init();
    //固定显示区
    OLED_ShowString(0,0,"COVER STATUS:",12);////size:选择字体 12/16/24
    OLED_ShowString(0,12,"_____________",12);
    INFRARED_Init();

    
    while(1)
    {

        uart_init(9600);
        printf("%d\n",PBin(10));
        INFRARED_Dislay(INFRARED_Detect());
        OLED_Refresh_Gram();
        delay_ms(200);
    }

}