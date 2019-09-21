#include "delay.h"
#include "key.h"
#include "led.h"
#include "beep.h"
#include "sys.h"
//��������ʵ��
int main(void){
    //����
    vu8 key=0;
    
    //��ʼ��
    delay_init();
    LED_Init();
    KEY_Init();
    BEEP_Init();
    LED0=0;//�ȵ������
    //����
 
    while(1){
        key=KEY_Scan(0);//һֱɨ�谴��
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
