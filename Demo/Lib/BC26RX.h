#include "stdint.h"
#include "Timer.h"
#include "Task.h"
#ifndef __BC26COMM_H__
#define __BC26COMM_H__

#define BC26RX_MAXBUFFER 128	
#define BC26RX_MAXROW 16
#define BC26RX_END 0
#define BC26RX_START 1
#define BC26RX_NO_ERROR 0
#define BC26RX_ERROR 1
#define BC26RX_SPECIALRX_NEW 1

extern uint8_t BC26RxBuffer[BC26RX_MAXROW][BC26RX_MAXBUFFER];
extern uint8_t BC26RxLength[BC26RX_MAXROW];
extern uint8_t BC26RxError[BC26RX_MAXROW];
extern int8_t BC26RxRow;

extern uint8_t BC26RxSpecialRX[BC26RX_MAXROW][BC26RX_MAXBUFFER];
extern uint8_t BC26RxSpecialRXLength[BC26RX_MAXROW];
extern uint8_t BC26RxSpecialRXError[BC26RX_MAXROW];
extern uint8_t BC26RxSpecialRXNew[BC26RX_MAXROW];
extern int8_t BC26RxSpecialRXRow;

extern uint8_t BC26RxATStartRow;
extern uint8_t BC26RxATEndRow;
extern uint8_t BC26RxATStatus;

extern Task BC26ResponseCheckTask;

void BC26ResponseCheck(Task *TaskAddr);

void BC26DataHandle(uint8_t RXData);
void BC26Comm(uint8_t *CommandAsk,uint8_t *Timeout,Timer *CheckTimer,uint8_t *Datas,uint16_t TimeoutTime,uint16_t TaskPeriod);
void BC26Comm_f(uint8_t *CommandAsk,uint8_t *Timeout,Timer *CheckTimer,uint16_t TimeoutTime,uint16_t TaskPeriod,void (*Doing)());
void BC26CommPub(uint8_t *CommandAsk,uint8_t *Timeout,Timer *CheckTimer,uint16_t TimeoutTime,uint16_t TaskPeriod,uint8_t ID,uint8_t *Topic,uint8_t *Msg,uint8_t Qos,uint8_t Retain);
void BC26CommSub(uint8_t *CommandAsk,uint8_t *Timeout,Timer *CheckTimer,uint16_t TimeoutTime,uint16_t TaskPeriod,uint8_t ID,uint8_t *Topic,uint8_t Qos);
void BC26CommUnSub(uint8_t *CommandAsk,uint8_t *Timeout,Timer *CheckTimer,uint16_t TimeoutTime,uint16_t TaskPeriod,uint8_t *Topic,uint8_t Qos);

#endif
