#include "timer.h"

uint16_t INT_TIMES=0;

void TIMER6_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //①时钟TIM6使能
    //定时器 TIM6初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //设置时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割.基本定时器不用设置
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM6向上计数
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure); //②初始化 TIM6
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //③允许更新中断
    //中断优先级 NVIC 设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; //TIM6中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //先占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
    NVIC_Init(&NVIC_InitStructure); //④初始化 NVIC 寄存器 
}


void TIM6_IRQHandler(void) //TIM6 中断
{
    
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET ) //检查 TIM6 更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
        INT_TIMES++;
    }
    if(INT_TIMES>60000) INT_TIMES=0;//防止溢出
}

void TIM6_SetTime(uint16_t arr)
{

    TIMER6_Init(7199,9999);//每隔1s进入一次中断
    //TIM_Cmd(TIM6, ENABLE);
    
}
