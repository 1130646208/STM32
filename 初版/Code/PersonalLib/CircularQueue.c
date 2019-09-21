#include "CircularQueue.h"

void QueueAdd(CircularQueue* CircularQueueAddr)//��������������
{
	if(CircularQueueAddr->IsAdded==CIRCULARQUEUE_NO_ADDED){
		if((*(CircularQueueAddr->StartAddr))==CIRCULARQUEUE_NULL){//�����ǰ����Ϊ��
			(*(CircularQueueAddr->StartAddr))=CircularQueueAddr;//��ʼ�����ַΪ�������ַ
			CircularQueueAddr->NextOne=CircularQueueAddr;//��һ������Ϊ������
			CircularQueueAddr->PreviousOne=CircularQueueAddr;//ǰһ������Ϊ������
		}
		else{//���в�Ϊ��
			CircularQueueAddr->PreviousOne=(*(CircularQueueAddr->StartAddr))->PreviousOne;//�������ǰһ������Ϊ��ʼ�����ǰһ������
			CircularQueueAddr->NextOne=(*(CircularQueueAddr->StartAddr));//���������һ������Ϊ������
			(*(CircularQueueAddr->StartAddr))->PreviousOne->NextOne=CircularQueueAddr;//��ʼ�����ǰһ���������һ����������Ϊ������
			(*(CircularQueueAddr->StartAddr))->PreviousOne=CircularQueueAddr;//��ʼ�����ǰһ������Ϊ������
            
		}
		CircularQueueAddr->IsAdded=CIRCULARQUEUE_ADDED;
	}
}
void QueueSub(CircularQueue* CircularQueueAddr)//ɾ������������
{
	if(CircularQueueAddr->IsAdded==CIRCULARQUEUE_ADDED){
		if((CircularQueueAddr->NextOne)==CircularQueueAddr)//���ֻ��һ������
			(*(CircularQueueAddr->StartAddr))=CIRCULARQUEUE_NULL;//���ø��������Ϊ��
		else{
			CircularQueueAddr->PreviousOne->NextOne=CircularQueueAddr->NextOne;//�������ǰһ���������һ������Ϊ���������һ������
			CircularQueueAddr->NextOne->PreviousOne=CircularQueueAddr->PreviousOne;//���������һ�������ǰһ������Ϊ�������ǰһ������
		}
		if(CircularQueueAddr==(*(CircularQueueAddr->StartAddr)))//���ɾ����������ͷ����
			(*(CircularQueueAddr->StartAddr))=CircularQueueAddr->NextOne;//�����������һ����������Ϊͷ
		CircularQueueAddr->IsAdded=CIRCULARQUEUE_NO_ADDED;
	}
}
