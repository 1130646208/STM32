#ifndef __CHARNUM_H__
#define __CHARNUM_H__

#include "stdint.h"

#define INT_TYPE int32_t	//ѡ����Ҫ������
#define FLOAT_TYPE double //ѡ����Ҫ�ĸ�����

int ChangeIntToChar(INT_TYPE Number,char Save[]);
int ChangeFloatToChar(FLOAT_TYPE Number,unsigned int Points,char Save[]);

INT_TYPE ChangeCharToInt(char Save[]);
FLOAT_TYPE ChangeCharToFloat(char Save[]);

#endif
