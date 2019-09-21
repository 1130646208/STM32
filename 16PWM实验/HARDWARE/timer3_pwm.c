#include "timer3_pwm.h"



void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{
    
    GPIO_InitTypeDef GPIO_InitStru;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
    TIM_OCInitTypeDef TIM_OCInitStru;
    /*GPIO����*/
    GPIO_InitStru.GPIO_Mode=GPIO_Mode_AF_PP;

    GPIO_InitStru.GPIO_Pin=GPIO_Pin_5;
    GPIO_InitStru.GPIO_Speed=GPIO_Speed_50MHz;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);// TIM3_CH2 ��ӳ�䵽 PB5 �ϡ�
    
    GPIO_Init(GPIOB,&GPIO_InitStru);
    /*��ʱ������*/
    TIM_TimeBaseInitStru.TIM_ClockDivision=0;
    TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStru.TIM_Period=arr;
    TIM_TimeBaseInitStru.TIM_Prescaler=psc;
    //TIM_TimeBaseInitStru.TIM_RepetitionCounter������
    
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStru);
    /*����TIM3_CH2 PWMģʽ*/
    TIM_OCInitStru.TIM_OCMode=TIM_OCMode_PWM2;
    TIM_OCInitStru.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStru.TIM_OutputState=TIM_OutputState_Enable;
    
    TIM_OC2Init(TIM3,&TIM_OCInitStru);
    
    /*ʹ��TIM3*/
    
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//???ʹ��Ԥװ�ؼĴ���
    TIM_Cmd(TIM3,ENABLE);
    
    
}
