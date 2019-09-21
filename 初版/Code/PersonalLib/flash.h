#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f10x.h"
#include "stm32f10x_flash.h"


extern uint8_t DevicesID[25];

void InitDevicesDatas(void);
void ReWriteDatas(void);
void FirstRunning(void);


#endif
