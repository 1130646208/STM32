/*
    �������Թܶ�Ӧ����
    ����Թܼ�⵽�ϰ�����͵�ƽ0V
    ���ϰ�����ߵ�ƽ3.3V
*/

#ifndef __INFRARED_H
#define __INFRARED_H

/*������ ��Ƭ����⵽�͵�ƽ
���� ��⵽�ߵ�ƽ*/
#define INFRARED_IS_THERE 0
#define INFRARED_NOT_THERE 1


#include "sys.h"
#define INFRARED PBin(10)



void INFRARED_Init(void);

u8 INFRARED_Detect(void);

void INFRARED_Dislay(u8 status);



#endif