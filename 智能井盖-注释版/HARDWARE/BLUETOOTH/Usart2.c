#include "Usart2.h"

uint8_t ReceiveBufferTemp[3][RX_BUFFER_SIZE];
uint8_t ReceiveBuffer[3][RX_BUFFER_SIZE_X_4];
uint16_t ReceiveNum[3],ReadStart[3];
/*
    ���ܣ���ʼ������2 DMAͨ��
    ����ֵ����
*/
void Usart2Init(void)
{
	USART_InitTypeDef UsartInit;
	NVIC_InitTypeDef NVICUsart;
	GPIO_InitTypeDef GPIOInit;
	DMA_InitTypeDef DMA_InitStructure;
	USART_DeInit(USART2);
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIOInit.GPIO_Pin=GPIO_Pin_2;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInit);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
	GPIOInit.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIOInit);
	
	USART_DeInit(USART2);
	UsartInit.USART_BaudRate=115200;		//������
	UsartInit.USART_WordLength=USART_WordLength_8b;		//��λ����
	UsartInit.USART_StopBits=USART_StopBits_1;		//һλֹͣλ
	UsartInit.USART_Parity=USART_Parity_No;		//��У��
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//��Ӳ������
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		//�շ�ģʽ
	USART_Init(USART2,&UsartInit);	
	
	NVICUsart.NVIC_IRQChannel=USART2_IRQn;		//�����ж�
	NVICUsart.NVIC_IRQChannelPreemptionPriority=0;	//�����ж����ȼ�
	NVICUsart.NVIC_IRQChannelSubPriority=0;
	NVICUsart.NVIC_IRQChannelCmd=ENABLE;		//ʹ���ж�����
	NVIC_Init(&NVICUsart);		
	
	USART_ClearFlag(USART2,USART_FLAG_TC);		//����жϱ�־λ
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);	
	USART_Cmd(USART2,ENABLE);		
	
	DMA_Cmd(DMA1_Channel6,DISABLE);
	DMA_DeInit(DMA1_Channel6);   //��DMA��ͨ��5�Ĵ�������Ϊȱʡֵ  ����1��Ӧ����DMAͨ��5
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&ReceiveBufferTemp[1][0]);  //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
    DMA_InitStructure.DMA_BufferSize = RX_BUFFER_SIZE;  //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //��������������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(DMA1_Channel6, ENABLE); 
}

/*
    ���ܣ�����2�жϷ�������������2�յ�����Ϣͨ��DMAͨ��ֱ��д��flash
    ����ֵ����
*/
void USART2_IRQHandler()
{
	static uint16_t Counts,Index=0,i;
	static uint8_t MeetNR;
	if(USART_GetITStatus(USART2,USART_IT_IDLE)!=RESET){
        
		DMA_Cmd(DMA1_Channel6,DISABLE);
		USART_ReceiveData(USART2);
		Counts=RX_BUFFER_SIZE-DMA1_Channel6->CNDTR;
		if((Index+Counts)>=RX_BUFFER_SIZE_X_4){
			ReceiveBuffer[1][Index]='\0';
			Index=0;
		}
		MeetNR=0;
		for(i=0;i<Counts;i++){
			if((ReceiveBufferTemp[1][i]=='\r')||(ReceiveBufferTemp[1][i]=='\n')){
				if(MeetNR==0){
					MeetNR=1;
					ReceiveBuffer[1][Index]='\n';
					Index++;
				}
			}
			else{
				MeetNR=0;
				ReceiveBuffer[1][Index]=ReceiveBufferTemp[1][i];
				Index++;
			}
		}
		ReceiveBuffer[1][Index]='\0';
		if(Index==RX_BUFFER_SIZE_X_4-1)
			Index=0;
		else
			Index++;
		ReceiveNum[1]++;
		DMA_SetCurrDataCounter(DMA1_Channel6,RX_BUFFER_SIZE);
		DMA_Cmd(DMA1_Channel6,ENABLE);
		USART_ClearITPendingBit(USART2,USART_IT_IDLE);

	}DEBUG("\r\nusart2:\r\n");DEBUG(ReceiveBufferTemp[1]);
}