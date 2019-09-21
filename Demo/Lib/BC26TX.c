#include "BC26TX.h"
#include "Usart.h"
#include "BC26RX.h"
#include "DEBUG.h"

uint8_t BC26TxBuffers[129];
uint8_t BC26TxBuffersStatus=BC26TX_BUFFER_EMPTY;
Task BC26TxAtCommandTask;

void BC26TxAtCommand(Task *TaskAddr)
{
    //���µĻ�������
	if(BC26TxBuffersStatus==BC26TX_BUFFER_NEW){
        //״̬��Ϊ����ѯ��
		BC26TxBuffersStatus=BC26TX_BUFFER_ASKING;
        //��ʼ����
		BC26RxATStatus=BC26RX_START;
		UsartSentDatas(USART3,BC26TxBuffers);
	}
    //AT�������ϣ����������ready
	else if((BC26TxBuffersStatus==BC26TX_BUFFER_ASKING)&&(BC26RxATStatus==BC26RX_END)){
		BC26TxBuffersStatus=BC26TX_BUFFER_READY;
	}
}
