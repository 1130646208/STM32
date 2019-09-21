#include "stm32f10x.h"
#include "infrared.h"
#include "oled.h"

void INFRARED_Init(void)//红外对管引脚初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    
    GPIO_Init(GPIOB,&GPIO_InitStructure);//经过初始化默认为低电平
}


u8 INFRARED_Detect(void)//井盖还在返回1，不在返回0
{
    
    if(PBin(10)==INFRARED_IS_THERE)
    return INFRARED_IS_THERE;
    else return INFRARED_NOT_THERE;
}

void INFRARED_Dislay(u8 status)
{
    OLED_Fill(0,24,72,48,0);// 刷新动态数据区
    if(status==INFRARED_IS_THERE)
    {
        OLED_ShowString(0,24,"SAFE",24);
        
    }else{
    
        OLED_ShowString(0,24,"STOLED",24);
        
    }  
}