#ifndef __KEY_H
#define __KEY_H

#include "sys.h"

#define KEY0 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//按键0
#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)//按键1
#define WK_UP GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//按键0  /*千古错误，不能加分号，会导致#18 excepted a “）”错误*/

#define KEY0_PRES 1
#define KEY1_PRES 2
#define WKUP_PRES 3


void KEY_Init(void);
u8 KEY_Scan(u8 mode);

#endif
