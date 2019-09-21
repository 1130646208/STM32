/*
    定义红外对管对应引脚
    红外对管检测到障碍输出低电平0V
    无障碍输出高电平3.3V
*/

#ifndef __INFRARED_H
#define __INFRARED_H

/*井盖在 单片机检测到低电平
不在 检测到高电平*/
#define INFRARED_IS_THERE 0
#define INFRARED_NOT_THERE 1


#include "sys.h"
#define INFRARED PBin(10)



void INFRARED_Init(void);

u8 INFRARED_Detect(void);

void INFRARED_Dislay(u8 status);



#endif