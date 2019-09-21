#include "stm32f10x.h"
#include "delay.h"
 #include "usart.h"
/************************************************
 ALIENTEK ��ӢSTM32F103������ʵ��0
 ����ģ��
 ע�⣬�����ֲ��е��½������½�ʹ�õ�main�ļ� 
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/
u8 pub;
static u8 it_times=0;

void TIM3_Int_Init(u16 arr,u16 psc)
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
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //�����ȼ� 3 ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ ͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure); //�ܳ�ʼ�� NVIC �Ĵ���
    
}
//��ʱ�� 3 �жϷ�������
void TIM3_IRQHandler(void) //TIM3 �ж�
{
    
    
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) //��� TIM3 �����жϷ������
    {
        it_times++;
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //��� TIM3 �����жϱ�־
        if(it_times==1){
        pub++;
        printf("%d",pub);
        it_times=0;}
    }//printf("%d",it_times);
}


int main()
{

    delay_init(); //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //���� NVIC �жϷ��� 2
    uart_init(115200); //���ڳ�ʼ��������Ϊ 115200
    TIM3_Int_Init(4999,7199); //10Khz �ļ���Ƶ�ʣ������� 5000 Ϊ 500ms
    TIM_Cmd(TIM3, ENABLE); //��ʹ�� TIM3
    while(1)
    {
        
      //  delay_ms(200);
    }
}
    
 