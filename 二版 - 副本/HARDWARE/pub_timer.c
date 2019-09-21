#include "pub_timer.h"
#include "SystemInit.h"

static u8 it_times=0;
static u8 pub=1;//0不发布





void pub_timer_init(u16 arr,u16 psc)
{
    
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //①时钟 TIM3 使能
    //定时器 TIM3 初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM 向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //②初始化 TIM3
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //③允许更新中断
    //中断优先级 NVIC 设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3 中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级 0 级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //从优先级 3 级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
    NVIC_Init(&NVIC_InitStructure); //④初始化 NVIC 寄存器
    
    //TIM_Cmd(TIM3, ENABLE); //⑤使能 TIM3
}


void start_pub(void)
{
    pub_timer_init(4999,7199);
    TIM_Cmd(TIM3, ENABLE); //⑤使能 TIM3
    
}
void stop_pub(void)
{
    TIM_Cmd(TIM3, DISABLE); //⑤使能 TIM3
    MQTTPublish[0].PublishOrNo=0;
    it_times=0;
    
}
//定时发送，每隔5s发送一次
void TIM3_IRQHandler(void) //TIM3 中断
{
    
    static u8 N=10;
    static u8 temp=0;
    if ((TIM_GetITStatus(TIM3,TIM_IT_Update) != RESET)) //检查 TIM3 更新中断发生与否
    {  
        
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清除 TIM3 更新中断标志
        if(PUB_Times!=0) {MQTTPublish[0].PublishOrNo=0;PUB_Times=0;}
        if(BC26SuccessConnect==1){
            it_times++;//开始计时
            if(pub==1 && it_times==N){//第五秒钟发送
                
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
