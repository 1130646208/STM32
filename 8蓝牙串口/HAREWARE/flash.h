#ifndef __FLASH_H__
#define __FLASH_H__

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#define SYSTEM_START_RCC RCC_APB2Periph_GPIOB
#define SYSTEM_START_PORT GPIOB
#define SYSTEM_START_PIN GPIO_Pin_15

#define SYSTEM_CAN_START GPIO_ReadInputDataBit(SYSTEM_START_PORT,SYSTEM_START_PIN)

extern uint8_t IPAddr[16];
extern uint8_t Port[6];
extern uint8_t ClientID[17];
extern uint8_t Username[17];
extern uint8_t Password[17];
extern uint8_t BTName[17];
extern uint8_t HeartTimes[4];
extern uint8_t DevicesID[25];
//extern uint8_t BTPassword[17];

void WriteFlashOneWord(uint32_t WriteAddress, uint32_t WriteData);
void WriteFlashDatas(uint32_t WriteAddress, uint8_t data[], uint32_t num);
int ReadFlashBtyes(uint32_t ReadAddress, uint8_t *ReadBuf, int32_t ReadNum);
void ReadFlashData(uint32_t ReadAddress, uint8_t *dest_Data, uint32_t num);
void ReadFlashString(uint8_t *Datas,uint8_t Index);
void ReadFlashSeriesStrings(uint8_t *Datas,uint8_t Start);
void InitDevicesDatas(void);

void SystemStartCheckGPIOInit(void);
void ReWriteDatas();

#endif