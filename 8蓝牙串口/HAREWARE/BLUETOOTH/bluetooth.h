#ifndef __BLUETOOTH_H__
#define __BLUETOOTH_H__

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

#define BLUETOOTH_PORT_RCC RCC_APB2Periph_GPIOA

#define BLUETOOTH_PORT GPIOA
#define BLUETOOTH_AT_PIN GPIO_Pin_6

#define BLUETOOTH_ENTER_AT GPIO_SetBits(BLUETOOTH_PORT,BLUETOOTH_AT_PIN)
#define BLUETOOTH_LEAVE_AT GPIO_ResetBits(BLUETOOTH_PORT,BLUETOOTH_AT_PIN)

extern uint8_t BTPassword[17];
extern uint8_t BTName[17];

void BTGPIOInit(void);


#endif