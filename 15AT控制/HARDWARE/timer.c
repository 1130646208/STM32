#include "timer.h"

uint16_t INT_TIMES=0;

void TIMER6_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //��ʱ��TIM6ʹ��
    //��ʱ�� TIM6��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler = psc; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�.������ʱ����������
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM6���ϼ���
    TIM_TimeBaseInit(TIM6,&TIM_TimeBaseStructure); //�ڳ�ʼ�� TIM6
    TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE ); //����������ж�
    //�ж����ȼ� NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn; //TIM6�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3; //��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); //�ܳ�ʼ�� NVIC �Ĵ��� 
}


void TIM6_IRQHandler(void) //TIM6 �ж�
{
    
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) == SET ) //��� TIM6 �����жϷ������
    {
        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
        INT_TIMES++;
    }
    if(INT_TIMES>60000) INT_TIMES=0;//��ֹ���
}

void TIM6_SetTime(uint16_t arr)
{

    TIMER6_Init(7199,9999);//ÿ��1s����һ���ж�
    //TIM_Cmd(TIM6, ENABLE);
    
}
