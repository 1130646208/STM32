#include "RTC.h"
#include "Usart.h"
#include "DEBUG.h"
void RTCInit(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR|RCC_APB1Periph_BKP,ENABLE);
	PWR_BackupAccessCmd(ENABLE);
	BKP_DeInit();
	RCC_LSEConfig(RCC_LSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	RCC_RTCCLKCmd(ENABLE);
	RTC_WaitForLastTask();
	RTC_WaitForSynchro();
	RTC_ITConfig(RTC_IT_SEC, ENABLE);
	RTC_WaitForLastTask();
	RTC_EnterConfigMode();
	RTC_SetPrescaler(32767);
	RTC_WaitForLastTask();
	RTC_SetCounter(0);
	RTC_WaitForLastTask();
	RTC_SetAlarm(5);
	RTC_WaitForLastTask();
	RTC_ITConfig(RTC_IT_ALR, ENABLE);
	RTC_WaitForLastTask();
	
	EXTI_InitStructure.EXTI_Line=EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode=EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd=ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTCAlarm_IRQn;	
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
	NVIC_Init(&NVIC_InitStructure);	
}
void RTCAlarm_IRQHandler(void)
{	
	EXTI_ClearITPendingBit(EXTI_Line17);
	RTC_ClearITPendingBit(RTC_IT_ALR);
	SystemInit();
	//DEBUG("Enter\r\n");
	RTC_SetCounter(0);
	RTC_WaitForLastTask();
	RTC_SetAlarm(5);
	RTC_WaitForLastTask();
		  	    						 	   	 
}
