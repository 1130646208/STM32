#include "stm32f10x.h"
#include "delay.h"
 #include "usart.h"
/************************************************
 ALIENTEK 精英STM32F103开发板实验0
 工程模板
 注意，这是手册中的新建工程章节使用的main文件 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 pub;
static u8 it_times=0;

void TIM3_Int_Init(u16 arr,u16 psc)
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
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3; //从优先级 3 级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ 通道被使能
    NVIC_Init(&NVIC_InitStructure); //④初始化 NVIC 寄存器
    
}
//定时器 3 中断服务程序⑥
void TIM3_IRQHandler(void) //TIM3 中断
{
    
    
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET ) //检查 TIM3 更新中断发生与否
    {
        it_times++;
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update ); //清除 TIM3 更新中断标志
        if(it_times==1){
        pub++;
        printf("%d",pub);
        it_times=0;}
    }//printf("%d",it_times);
}


int main()
{

    delay_init(); //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置 NVIC 中断分组 2
    uart_init(115200); //串口初始化波特率为 115200
    TIM3_Int_Init(4999,7199); //10Khz 的计数频率，计数到 5000 为 500ms
    TIM_Cmd(TIM3, ENABLE); //⑤使能 TIM3
    while(1)
    {
        
      //  delay_ms(200);
    }
}
    
 