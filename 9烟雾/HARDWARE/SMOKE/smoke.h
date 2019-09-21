#ifndef _mq4_H
#define _mq4_H

#include "stm32f10x.h"
#include "Delay.h"
extern  uint8_t Gas_Condition;//判断CO参数状态，状态正常正常发送信息，状态错误，瞬间发送信息。0异常 1正常




void mq4_init(void);
float get_co(void);
float get_co_value(void);
float get_value(void);
float gas_level_status();
uint8_t * get_real_data(float data);
void Decide_Gas_ERROR(float data);

/*wsx*/
void SMOKE_Display(const u8* status);
#endif