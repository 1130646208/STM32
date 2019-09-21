#include "stm32f10x.h"
#include "stm32f10x_flash.h"
#ifndef __FLASH_H__
#define __FLASH_H__

extern uint8_t IPAddr[16];
extern uint8_t Port[6];
extern uint8_t ClientID[17];
extern uint8_t Username[17];
extern uint8_t Password[17];
extern uint8_t Status[17];
extern uint8_t HeartTimes[6];
extern uint8_t DevicesID[25];
extern uint8_t BTPassword[17];
extern uint8_t AuthCode[17];
extern uint8_t IDChange;

void InitDevicesDatas(void);
void ReWriteDatas(void);
uint16_t StringLength(uint8_t *String);
uint8_t StringEqual(uint8_t *String1,uint8_t *String2);
void GETDevicesID(void);

#endif
