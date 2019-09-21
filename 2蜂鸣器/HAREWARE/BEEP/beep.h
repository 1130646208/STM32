#ifndef __BEEP_H

#define __BEEP_H
#include "sys.h"//位带操作
#include "stm32f10x.h"

//蜂鸣器端口定义
#define BEEP PBout(8)
//初始化
void BEEP_Init (void);

#endif
