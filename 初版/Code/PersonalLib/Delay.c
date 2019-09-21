#include "Delay.h"

void DelayMs(uint32_t times)
{
	uint32_t temp;
	SysTick->LOAD=times*9000;
	SysTick->VAL=0X00;
	SysTick->CTRL=0X01;
	do{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SystickStops();
}
void DelayUs(uint32_t times)
{
	uint32_t temp;
	SysTick->LOAD=times*9;
	SysTick->VAL=0X00;
	SysTick->CTRL=0X01;
	do{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));
	SystickStops();
}
void DelayInit(void)
{
	SysTick->CTRL&=~(1<<2);
}
void SystickStops(void)
{
	SysTick->CTRL=0X00;
	SysTick->VAL=0X00;
}
