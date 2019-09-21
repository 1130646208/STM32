#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "Usart.h"
#include "showSystem.h"

#define DEBUG_YES 1
#define DEBUG_NO 0

#define NEED_DEBUG DEBUG_YES

#if NEED_DEBUG==DEBUG_YES
	#define DEBUG(X) UsartSentDatas(USART1,X)
//    #define DEBUG(X) show_sys(X)
//    #define DEBUG2(X) show_sys2(X)
//    #define DEBUG0(X) show_sys0(X)

    
#else
	#define DEBUG(X) 
#endif

#endif
