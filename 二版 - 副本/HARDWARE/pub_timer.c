#include "pub_timer.h"
#include "SystemInit.h"

static u8 it_times=0;
static u8 pub=1;//0������





void pub_timer_init(u16 arr,u16 psc)
{
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //��ʱ�� TIM3 ʹ��
    //��ʱ�� TIM3 ��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����ʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM ���ϼ���
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //�ڳ�ʼ�� TIM3
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //����������ж�
    //�ж����ȼ� NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 �ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ� 0 ��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ� 3 ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); //�ܳ�ʼ�� NVIC �Ĵ���
    
    //TIM_Cmd(TIM3, ENABLE); //��ʹ�� TIM3
}


void start_pub(void)
{
    pub_timer_init(4999,7199);
    TIM_Cmd(TIM3, ENABLE); //��ʹ�� TIM3
    
}
void stop_pub(void)
{
    TIM_Cmd(TIM3, DISABLE); //��ʹ�� TIM3
    MQTTPublish[0].PublishOrNo=0;
    it_times=0;
    
}
//��ʱ���ͣ�ÿ��5s����һ��
void TIM3_IRQHandler(void) //TIM3 �ж�
{
    
    static u8 N=10;
    static u8 temp=0;
    if ((TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)) //��� TIM3 �����жϷ������
    {  
        
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //��� TIM3 �����жϱ�־
        if(PUB_Times!=0) {MQTTPublish[0].PublishOrNo=0;PUB_Times=0;}
        if(BC26SuccessConnect==1){
            it_times++;//��ʼ��ʱ
            if(pub==1 && it_times==N){//�������ӷ���
                
                MQTTPublish[0].PublishOrNo=1;
                temp=it_times;

            }else if(it_times-temp == N)
            {
                pub=1;
                it_times=0;
                temp=0;
            }

        }else MQTTPublish[0].PublishOrNo=0;
        
//        
//        if(MQTTPublish[0].PublishOrNo==1) DEBUG("Pub:Yes.");
//        else DEBUG("Pub:No.");
//        if(BC26SuccessConnect) DEBUG("MQTTCon:YES.");
//        else DEBUG("MQTTCon:No.");
    }
}
