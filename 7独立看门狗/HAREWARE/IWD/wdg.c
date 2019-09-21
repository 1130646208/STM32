#include "wdg.h"
//初始化独立看门狗
//prer:分频数:0~7(只有低3位有效!)
//分频因子=4*2^prer.但最大值只能是256!
//rlr:重装载寄存器值:低11位有效.
//时间计算(大概):Tout=((4*2^prer)*rlr)/40 (ms).




void IWDG_Init(u8 prer,u16 rlr)
{

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    
    IWDG_SetPrescaler(prer);
    
    IWDG_SetReload(rlr);
    
    IWDG_ReloadCounter();
    
    IWDG_Enable();
    
}

void IWDG_Feed(void)
{
    IWDG_ReloadCounter();
}