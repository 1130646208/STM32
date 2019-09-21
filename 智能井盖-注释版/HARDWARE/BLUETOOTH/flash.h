#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f10x.h"
#include "Usart2.h"
#include "stm32f10x_flash.h"
#include "DEBUG.h"
#include "BC26Task.h"


//extern uint8_t DevicesID[25];
//extern char IPAddr[16];//={"47.106.249.19"};
//extern char Port[6];//={"61613"};
//extern char IPAddr[16];
//extern char Port[6];

void InitDevicesDatas(void);
void ReWriteDatas(void);
void FirstRunning(void);

u8 SetNetInfo(void);
void ClearNetInfo(void);


#endif
