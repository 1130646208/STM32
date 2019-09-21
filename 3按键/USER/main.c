#include "delay.h"
#include "key.h"
#include "led.h"
#include "beep.h"
#include "sys.h"
//按键输入实验
int main(void){
    //定义
    vu8 key=0;
    
    //初始化
    delay_init();
    LED_Init();
    KEY_Init();
    BEEP_Init();
    LED0=0;//先点亮红灯
    //功能
 
    while(1){
        key=KEY_Scan(0);//一直扫描按键
        if (key)
        {
        
            switch(key)
            {
                case WKUP_PRES:
                    BEEP=!BEEP;
                break;
                case KEY1_PRES:
                    LED1=!LED1;
                break;
                case KEY0_PRES:
                    LED0=!LED0;
                    LED1=!LED1;
                break;
            }
   
        } else delay_ms(10);
    } 
return 0;
}
