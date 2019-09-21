#ifndef __SYSTEMINIT_H__
#define __SYSTEMINIT_H__

#include "stm32f10x.h"
#include "BC26RX.h"
#include "BC26TX.h"
#include "RTC.h"
#include "Task.h"
#include "Delay.h"
#include "Timer.h"
#include "Usart.h"
#include "BC26.h"
#include "LowPower.h"
#include "BC26Task.h"
#include "flash.h"


/**/
#include "test1.h"
#include "dataSum.h"
#include "pub_timer.h"


void SystemHardwareInit(void);
extern CircularQueue *SystemStartTask;
extern char StringTemp[128];


void sen_test(void);

#endif

