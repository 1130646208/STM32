#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "Task.h"
#ifndef __BC26_H__
#define __BC26_H__
#define BC26_RESET_PORT_RCC RCC_APB2Periph_GPIOA
#define BC26_RESET_PORT GPIOA
#define BC26_RESET_PIN GPIO_Pin_8

#define BC26_COMMAND_WAIT_ASK 0
#define BC26_COMMAND_READY_ASK 1
#define BC26_COMMAND_ASKING 2

extern CircularQueue *BC26StartTask;

void BC26GpioInit(void);
void BC26Reset(void);
void BC26TaskInit(void);
void BC26Connect(void);
#endif
