#ifndef _water_H
#define _water_H

#include "stm32f10x.h"
#include "Delay.h"
#define ALCH       250       //醉驾标准80mg/L
//K_MG_MV和K_ZERO为传感器校准系数，要根据每个MQ-3模块校准
#define K_MG_MV    160/66  //传感器灵敏度系数，每毫克/L对应的10毫伏数
#define K_ZERO     28      //传感器零点漂移，约130mV

extern  u8 Water_Condition;//判断水位参数状态，状态正常正常发送信息，状态错误，瞬间发送信息。0异常 1正常
void water_init(void);

u8 get_water(void)	;
u8 get_water_data(void);
u8 * get_water_state(u8 data);
void Decide_Water_ERROR(int data);

void choose_water(u8 choose);
#endif