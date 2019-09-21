#ifndef __DEBUG_H__
#define __DEBUG_H__
#include "Usart.h"

#define DEBUG_YES 1
#define DEBUG_NO 0

#define NEED_DEBUG DEBUG_YES

#if NEED_DEBUG==DEBUG_YES
	#define DEBUG(X) UsartSentDatas(USART1,X) 
#else
	#define DEBUG(X) 
#endif

#endif