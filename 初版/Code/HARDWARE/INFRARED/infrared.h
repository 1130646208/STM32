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
#define INFRARED PBin(9)
extern  u8 INFRARE_Condition;


void infrared_init(void);

u8 infrared_detect(void);

void Decide_Inf_ERROR(void);
    
void infrared_dislay(u8 status);



#endif
