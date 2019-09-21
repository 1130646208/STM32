#include "BC26TX.h"
#include "BC26RX.h"
#include "Usart.h"
#include "Timer.h"
#include "DEBUG.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"

void BC26TxAtCommand(Task *TaskAddr);
void BC26TXRequest(uint8_t (*)(void),char *ATCommand,...);
void BC26TXFinish(void);
void BC26TXRepeat(void);

BC26TXTypeDef BC26TX={.BuffersStatus=BC26TX_BUFFER_EMPTY,
            .TXTask=BC26TxAtCommand,.TXRequest=BC26TXRequest,
            .TXFinish=BC26TXFinish,.TXRepeat=BC26TXRepeat};

            
void BC26TXATCommandViaUsart()
{
	UsartSentDatas(USART3,BC26TX.Buffer);
}
void BC26TxAtCommand(Task *TaskAddr)
{
	static Timer CheckTimer;
	uint8_t ATResultHandleResult=0;
	if(BC26TX.BuffersStatus==BC26TX_BUFFER_NEW){
		BC26TX.BuffersStatus=BC26TX_BUFFER_ASKING;
		BC26RX.ATCOMStatus=BC26_ATCOM_ASK;
		BC26TXATCommandViaUsart();
        
	}
	else if((BC26TX.BuffersStatus==BC26TX_BUFFER_ASKING)&&(BC26RX.ATCOMStatus==BC26_ATCOM_RESPOND)){
		ATResultHandleResult=BC26TX.ATResultHandle();
		if(ATResultHandleResult==BC26TX_FINISH)
			BC26TX.TXFinish();
		else if(ATResultHandleResult==BC26TX_REPEAT)
			BC26TX.TXRepeat();
	}
}
void BC26TXRequest(uint8_t (*ATResultHandle)(void),char *ATCommand,...)
{
	va_list args;
	uint16_t i;
	if(BC26TX.BuffersStatus==BC26TX_BUFFER_EMPTY){
		BC26TX.BuffersStatus=BC26TX_BUFFER_NEW;
		va_start(args,ATCommand);
		vsprintf(BC26TX.Buffer,ATCommand,args);
		va_end(args);
		BC26TX.ATResultHandle=ATResultHandle;
	}
}
void BC26TXRepeat(void)
{
	BC26TX.BuffersStatus=BC26TX_BUFFER_NEW;
}
void BC26TXFinish(void)
{
	BC26TX.BuffersStatus=BC26TX_BUFFER_EMPTY;
}
