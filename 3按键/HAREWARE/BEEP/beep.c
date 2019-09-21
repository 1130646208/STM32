#include "beep.h"

#include "stm32f10x.h"
//初始化PB8并使能时钟

void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能GPIOB时钟
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;//BEEP-->GPIOB.8 端口配置
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//速度为50MHz
    
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    GPIO_ResetBits(GPIOB,GPIO_Pin_8);//关闭蜂鸣器输出
   
}
