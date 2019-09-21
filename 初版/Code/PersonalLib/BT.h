#ifndef __BT_H__
#define __BT_H__

#include "stm32f10x.h"

#define BLUETOOTH_PORT_RCC RCC_APB2Periph_GPIOB
#define BLUETOOTH_PORT GPIOB
#define BLUETOOTH_AT_PIN GPIO_Pin_7

#define BLUETOOTH_ENTER_AT GPIO_SetBits(BLUETOOTH_PORT,BLUETOOTH_AT_PIN)
#define BLUETOOTH_LEAVE_AT GPIO_ResetBits(BLUETOOTH_PORT,BLUETOOTH_AT_PIN)

extern uint8_t BTPassword[17];

void BTGPIOInit(void);
uint8_t SetBTPasswd(uint8_t *Receives);
void GenerateBTPassword(void);


#endif