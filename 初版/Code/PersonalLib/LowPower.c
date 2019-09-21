#include "LowPower.h"

void SystemEnterSleep(void)
{
	uint32_t TIM2CNT;
	TIM2CNT=TIM_GetCounter(TIM2);
	TIM2->DIER &= (uint16_t)~TIM_IT_Update;
	TIM_Cmd(TIM2,DISABLE);
	NVIC_SystemLPConfig(NVIC_LP_SLEEPDEEP,DISABLE);
	NVIC_SystemLPConfig(NVIC_LP_SLEEPONEXIT,DISABLE);
	__WFI();
	TIM_Cmd(TIM2,ENABLE);
	TIM_SetCounter(TIM2,TIM2CNT);
	TIM2->DIER |= TIM_IT_Update;
}
