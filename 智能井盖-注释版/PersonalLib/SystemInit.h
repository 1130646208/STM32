#ifndef __SYSTEMINIT_H__
#define __SYSTEMINIT_H__

#include "stm32f10x.h"
#include "BC26RX.h"
#include "BC26TX.h"
#include "Task.h"
#include "Delay.h"
#include "Timer.h"
#include "Usart.h"
#include "BC26.h"
#include "BC26Task.h"

/*****************************/
#include "dataSum.h"
#include "pub_timer.h"
#include "flash.h"
#include "test1.h"
/*****************************/


#define A2Z "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define a2z "abcdefghijklmnopqrstuvwxyz"
#define Z2N "0123456789"

void SystemHardwareInit(void);
extern CircularQueue *SystemStartTask;
extern char StringTemp[128];
extern char StringTemp1[128];
extern char StringTemp2[128];
//extern char DevicesID[25];
extern uint8_t PUB_Times;

#endif
