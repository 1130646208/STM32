#include "timer3_pwm.h"



void TIM3_PWM_Init(uint16_t arr,uint16_t psc)
{
    
    GPIO_InitTypeDef GPIO_InitStru;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStru;
    TIM_OCInitTypeDef TIM_OCInitStru;
    /*GPIO配置*/
    GPIO_InitStru.GPIO_Mode=GPIO_Mode_AF_PP;

    GPIO_InitStru.GPIO_Pin=GPIO_Pin_5;
    GPIO_InitStru.GPIO_Speed=GPIO_Speed_50MHz;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);

    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);// TIM3_CH2 重映射到 PB5 上。
    
    GPIO_Init(GPIOB,&GPIO_InitStru);
    /*定时器配置*/
    TIM_TimeBaseInitStru.TIM_ClockDivision=0;
    TIM_TimeBaseInitStru.TIM_CounterMode=TIM_CounterMode_Up;
    TIM_TimeBaseInitStru.TIM_Period=arr;
    TIM_TimeBaseInitStru.TIM_Prescaler=psc;
    //TIM_TimeBaseInitStru.TIM_RepetitionCounter不设置
    
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStru);
    /*设置TIM3_CH2 PWM模式*/
    TIM_OCInitStru.TIM_OCMode=TIM_OCMode_PWM2;
    TIM_OCInitStru.TIM_OCPolarity=TIM_OCPolarity_High;
    TIM_OCInitStru.TIM_OutputState=TIM_OutputState_Enable;
    
    TIM_OC2Init(TIM3,&TIM_OCInitStru);
    
    /*使能TIM3*/
    
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);//???使能预装载寄存器
    TIM_Cmd(TIM3,ENABLE);
    
    
}
