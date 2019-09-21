#include "stm32f10x.h"
#include "timer3_pwm.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

int main(void)
{
    u16 led0pwmval=0;
    u8 dir=1;
    delay_init(); //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //���� NVIC �жϷ��� 2
    uart_init(115200); //���ڳ�ʼ��������Ϊ 115200
    LED_Init(); //LED �˿ڳ�ʼ��
    TIM3_PWM_Init(899,0); //����Ƶ,PWM Ƶ��=72000/900=80Khz
    printf("Init OK!");
    
    while(1)
    {
        delay_ms(5);
        if(dir)led0pwmval++;
        else led0pwmval--;
        if(led0pwmval>600)dir=0;
        if(led0pwmval==0)dir=1;
        TIM_SetCompare2(TIM3,led0pwmval);
    }
}
