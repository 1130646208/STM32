#include "stm32f10x.h"
#include "infrared.h"
#include "oled.h"

u8 INFRARE_Condition;
void infrared_init(void)//红外对管引脚初始化
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    
    GPIO_Init(GPIOB,&GPIO_InitStructure);//经过初始化默认为低电平
}


u8 infrared_detect(void)//
{
    
    if(PBin(9)==INFRARED_IS_THERE)
    return 0;
    else return 1;
}

void Decide_Inf_ERROR()
{
    if(!infrared_detect())
        INFRARE_Condition=1;
    else
        INFRARE_Condition=0;
}
    

void infrared_dislay(u8 status)
{
    OLED_Fill(43,25,66,36,0);// 刷新动态数据区
    if(status==INFRARED_IS_THERE)
    {
        OLED_ShowString(43,25,"SAFE",12);

    }else{
        OLED_ShowString(43,25,"GONE",12); 
    }  
}
