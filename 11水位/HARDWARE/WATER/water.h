#ifndef _water_H
#define _water_H

#include "stm32f10x.h"
#include "Delay.h"
#define ALCH       250       //��ݱ�׼80mg/L
//K_MG_MV��K_ZEROΪ������У׼ϵ����Ҫ����ÿ��MQ-3ģ��У׼
#define K_MG_MV    160/66  //������������ϵ����ÿ����/L��Ӧ��10������
#define K_ZERO     28      //���������Ư�ƣ�Լ130mV

extern  u8 Water_Condition;//�ж�ˮλ����״̬��״̬��������������Ϣ��״̬����˲�䷢����Ϣ��0�쳣 1����
void water_init(void);

u8 get_water(void)	;
u8 get_water_data(void);
u8 * get_water_state(u8 data);
void Decide_Water_ERROR(int data);

void choose_water(u8 choose);
#endif