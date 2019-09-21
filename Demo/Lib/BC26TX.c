#include "BC26TX.h"
#include "Usart.h"
#include "BC26RX.h"
#include "DEBUG.h"

uint8_t BC26TxBuffers[129];
uint8_t BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
Task BC26TxAtCommandTask;

void BC26TxAtCommand(Task *TaskAddr)
{
    //有新的缓冲内容
	if(BC26TxBuffersStatus==BC26TX_BUFFER_NEW){
        //状态变为正在询问
		BC26TxBuffersStatus=BC26TX_BUFFER_ASKING;
        //开始发送
		BC26RxATStatus=BC26RX_START;
		UsartSentDatas(USART3,BC26TxBuffers);
	}
    //AT命令发送完毕，缓冲区变成ready
	else if((BC26TxBuffersStatus==BC26TX_BUFFER_ASKING)&&(BC26RxATStatus==BC26RX_END)){
		BC26TxBuffersStatus=BC26TX_BUFFER_READY;
	}
}
