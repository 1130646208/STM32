#ifndef __BEEP_H

#define __BEEP_H
#include "sys.h"//λ������
#include "stm32f10x.h"

//�������˿ڶ���
#define BEEP PBout(8)
//��ʼ��
void BEEP_Init (void);

#endif
