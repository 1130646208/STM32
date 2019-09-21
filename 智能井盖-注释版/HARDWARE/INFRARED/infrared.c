#include "stm32f10x.h"
#include "infrared.h"
#include "oled.h"
u8 INFRARE_Condition;//红外对管状态
/*
    功能：//红外对管引脚初始化
    返回值：无
*/
void infrared_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;    
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    //经过初始化默认为低电平
}

/*
    功能：检测红外对管前是否有障碍物
    返回值：有0 ，无1
*/
u8 infrared_detect(void)
{
    
    if(PBin(9)==INFRARED_IS_THERE)
    return 0;
    else return 1;
}
/*
    功能：判断红外状态是否异常
    返回值：无
*/
void Decide_Inf_ERROR()
{
    if(!infrared_detect())
        INFRARE_Condition=1;
    else
        INFRARE_Condition=0;
}
    
/*
    功能：将井盖是否安全显示在OLED上--即红外检测到障碍物显示安全，否则显示不安全
    返回值：无
*/
void infrared_dislay(u8 status)
{
    if(status==INFRARED_IS_THERE)
    {
        OLED_ShowString(43,25,"SAFE",12);

    }else{
        OLED_ShowString(43,25,"GONE",12); 
    }  
}
