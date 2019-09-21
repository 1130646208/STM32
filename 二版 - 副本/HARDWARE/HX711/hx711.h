#ifndef _hx711_H
#define _hx711_H

#include "stm32f10x.h"
#include "delay.h"
#include "OLED.h"
extern  uint8_t  hx711_Condition;//判断井盖压力参数状态，状态正常正常发送信息，状态错误，瞬间发送信息。0异常 1正常
extern u8 hx711_init_val;
/* 定时使用的IO口 */
//iic通信
//b6---daut(sck)数据端口 b7---scl时钟端口
#define I2C_SCL_H_A GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define I2C_SCL_L_A GPIO_ResetBits(GPIOB,GPIO_Pin_7)

#define I2C_SDA_H_A GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define I2C_SDA_L_A GPIO_ResetBits(GPIOB,GPIO_Pin_6)
//--------------------------------------------------------------------//

//a6---daut(sck)数据端口 a7---scl时钟端口
#define I2C_SCL_H_B GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define I2C_SCL_L_B GPIO_ResetBits(GPIOA,GPIO_Pin_7)

#define I2C_SDA_H_B GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define I2C_SDA_L_B GPIO_ResetBits(GPIOA,GPIO_Pin_6)
//----------------------------------------------------------------------------//
//a4---daut(sck)数据端口 a5---scl时钟端口
#define I2C_SCL_H_C GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define I2C_SCL_L_C GPIO_ResetBits(GPIOA,GPIO_Pin_5)

#define I2C_SDA_H_C GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define I2C_SDA_L_C GPIO_ResetBits(GPIOA,GPIO_Pin_4)
/* 声明全局函数 */
void hx711_init(void);

u8 get_hx711_data(void);
u8 Decide_Stress_ERROR(void);
void clear_hx711_data(void);
void hx711_calib(void);
void STRESS_Display(const u8* stress);

#endif
