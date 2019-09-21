#ifndef _mq4_H
#define _mq4_H

#include "stm32f10x.h"
#include "Delay.h"
#include "oled.h"
#include "myiic.h"
#include "Task.h"


extern  uint8_t Gas_Condition;//判断CO参数状态，状态正常正常发送信息，状态错误，瞬间发送信息。0异常 1正常

void smoke_init(void);
float get_smoke_data(void);
void clear_smoke_data(void);
u8 * get_smoke_level(float data);
void Decide_Smoke_ERROR(float data);

/*wsx*/
void SMOKE_Display(const u8* status);
#endif
