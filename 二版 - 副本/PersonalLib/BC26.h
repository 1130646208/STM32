#ifndef __BC26_H__
#define __BC26_H__

#include "stm32f10x.h"
#include "Task.h"

#define BC26_RESET_PORT_RCC RCC_APB2Periph_GPIOA
#define BC26_RESET_PORT GPIOA
#define BC26_RESET_PIN GPIO_Pin_8

extern Task BC26ResetTask;
extern uint8_t BC26ReInit;
void BC26ResetMission(Task *TaskAddr);

void BC26GpioInit(void);
void BC26Reset(void);
	
#endif
