#include "stm32f10x.h"
#include "key.h"
#include "sys.h"
#include "delay.h"


//按键初始化函数
void KEY_Init(void){

    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE,ENABLE);//IO时钟
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;//KEY0 KEY1 上拉模式
    GPIO_Init(GPIOE,&GPIO_InitStructure);//初始化成E.3,4
    
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPD;//WK_UP 下拉模式
    GPIO_Init(GPIOA,&GPIO_InitStructure);//初始化为A.0

}
//按键处理函数
//返回按键值，1=KEY0 2=KEY1 3=KEY3（WK_UP）
//mode 0 连按 1 不支持连按

u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//按键按松开标志
	if(mode)key_up=1;  //支持连按		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		DelayUs(10);//去抖动 
		key_up=0;
		if(KEY0==0)return KEY0_PRES;
		else if(KEY1==0)return KEY1_PRES;
		else if(WK_UP==1)return WKUP_PRES;
	}else if(KEY0==1&&KEY1==1&&WK_UP==0)key_up=1; 	    
 	return 0;// 无按键按下
}










