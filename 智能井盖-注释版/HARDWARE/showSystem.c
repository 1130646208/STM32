#include "showSystem.h"
/*
    功能：定义一些显示OLED的格式
    返回值：无
*/
void show_sys(unsigned char* str)
{
    OLED_ShowString(0,49,"        ",12);
    OLED_ShowString(0,49,str,12);
    OLED_Refresh_Gram();
    DelayMs(100);
}

void show_sys2(unsigned char* str)
{
    OLED_ShowString(49,49,"            ",12);
    OLED_ShowString(49,49,str,12);
    OLED_Refresh_Gram();
    DelayMs(100);
}

void show_sys0(unsigned char* str)
{
    OLED_ShowString(0,49,"                     ",12);
    OLED_ShowString(0,49,str,12);
    OLED_Refresh_Gram();
    DelayMs(100);
}