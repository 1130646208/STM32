#include "exti.h"
#include "key.h"
#include "led.h"
#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"

void EXTIX_Init(void)
{
    
    
    EXTI_InitTypeDef EXTI_InitCon;    
    NVIC_InitTypeDef NVIC_InitCon;
    
    /*1 ��ʼ��IO��Ϊ����,�ڰ���ʵ�����Ѿ���ʼ������*/
    KEY_Init();
    /*2 ����IO����ʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    /*3 ����IO�����ж���ӳ���ϵ*/
    /*4 ��ʼ�������жϣ����ô�������*/ 
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource4);
    EXTI_InitCon.EXTI_Line=EXTI_Line4;
    EXTI_InitCon.EXTI_Mode=EXTI_Mode_Interrupt;
    EXTI_InitCon.EXTI_Trigger=EXTI_Trigger_Falling;
    EXTI_InitCon.EXTI_LineCmd=ENABLE;
    EXTI_Init(&EXTI_InitCon);
    
    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE,GPIO_PinSource3);
    EXTI_InitCon.EXTI_Line=EXTI_Line3;
    EXTI_Init(&EXTI_InitCon);
    
    
    
    
    
    
    
    /*5 �����жϷ��� ����������*/
    /*6 ʹ���ж�*/
    NVIC_InitCon.NVIC_IRQChannel=EXTI4_IRQn;
    NVIC_InitCon.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitCon.NVIC_IRQChannelPreemptionPriority=1;
    NVIC_InitCon.NVIC_IRQChannelSubPriority=1;
    NVIC_Init(&NVIC_InitCon);
    
    NVIC_InitCon.NVIC_IRQChannel=EXTI3_IRQn;
    NVIC_InitCon.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitCon.NVIC_IRQChannelPreemptionPriority=2;
    NVIC_InitCon.NVIC_IRQChannelSubPriority=2;
    NVIC_Init(&NVIC_InitCon);
    
  
}

/*7 ��д�жϷ�����*/
    
void EXTI3_IRQHandler()
{
    delay_ms(10);
    if (KEY1==0)
    {
        LED0=!LED0;

    }
 
    EXTI_ClearITPendingBit(EXTI_Line3);
}
    
void EXTI4_IRQHandler()
{
    delay_ms(10);
    if (KEY0==0)
    {
        
        LED0=!LED0;
        LED1=!LED1;
    }
    EXTI_ClearITPendingBit(EXTI_Line4);

}




















