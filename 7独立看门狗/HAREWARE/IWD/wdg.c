#include "wdg.h"
//��ʼ���������Ź�
//prer:��Ƶ��:0~7(ֻ�е�3λ��Ч!)
//��Ƶ����=4*2^prer.�����ֵֻ����256!
//rlr:��װ�ؼĴ���ֵ:��11λ��Ч.
//ʱ�����(���):Tout=((4*2^prer)*rlr)/40 (ms).




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