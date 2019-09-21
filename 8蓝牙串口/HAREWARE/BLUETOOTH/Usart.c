#include "Usart.h"
#include "delay.h"

#include "DEBUG.h"

void Usart1Init(void)
{
	USART_InitTypeDef UsartInit;
	NVIC_InitTypeDef NVICUsart1;
	GPIO_InitTypeDef GPIOAInit;
	USART_DeInit(USART1);		
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);	
	
	GPIOAInit.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIOAInit.GPIO_Pin=GPIO_Pin_9;
	GPIOAInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOAInit);
	
	GPIOAInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
	GPIOAInit.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIOAInit);
	
	UsartInit.USART_BaudRate=115200;		
	UsartInit.USART_WordLength=USART_WordLength_8b;		
	UsartInit.USART_StopBits=USART_StopBits_1;		
	UsartInit.USART_Parity=USART_Parity_No;		
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		
	USART_Init(USART1,&UsartInit);	
	
	NVICUsart1.NVIC_IRQChannel=USART1_IRQn;		
	NVICUsart1.NVIC_IRQChannelPreemptionPriority=1;	
	NVICUsart1.NVIC_IRQChannelSubPriority=0;
	NVICUsart1.NVIC_IRQChannelCmd=ENABLE;		
	NVIC_Init(&NVICUsart1);		
	USART_ClearFlag(USART1,USART_FLAG_TC);		
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);		
	USART_Cmd(USART1,ENABLE);		
	
}
void Usart3Init(void)
{
	USART_InitTypeDef UsartInit;
	NVIC_InitTypeDef NVICUsart3;
	GPIO_InitTypeDef GPIOInit;
	USART_DeInit(USART3);		
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIOInit.GPIO_Pin=GPIO_Pin_10;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInit);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
	GPIOInit.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIOInit);
	
	UsartInit.USART_BaudRate=115200;		
	UsartInit.USART_WordLength=USART_WordLength_8b;		
	UsartInit.USART_StopBits=USART_StopBits_1;		
	UsartInit.USART_Parity=USART_Parity_No;		
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		
	USART_Init(USART3,&UsartInit);	
	
	NVICUsart3.NVIC_IRQChannel=USART3_IRQn;		
	NVICUsart3.NVIC_IRQChannelPreemptionPriority=0;	
	NVICUsart3.NVIC_IRQChannelSubPriority=0;
	NVICUsart3.NVIC_IRQChannelCmd=ENABLE;		
	NVIC_Init(&NVICUsart3);		
	USART_ClearFlag(USART3,USART_FLAG_TC);		
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);		
	USART_Cmd(USART3,ENABLE);		
}
void Usart2Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//����һ���ṹ�������������ʼ��GPIO

	USART_InitTypeDef  USART_InitStructure;	  //���ڽṹ�嶨��

	NVIC_InitTypeDef NVIC_InitStructure;//�жϽṹ�嶨��
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //��ʱ��
	
	/*  ����GPIO��ģʽ��IO�� */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX			   //�������PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //�����������
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* ��ʼ����������IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX			 //��������PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //ģ������
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* ��ʼ��GPIO */


	USART_InitStructure.USART_BaudRate=38400;   //����������Ϊ9600	//������
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		//���ݳ�8λ
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1λֹͣλ
	USART_InitStructure.USART_Parity=USART_Parity_No;				//��Ч��
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //ʧ��Ӳ����
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	 //�������ͺͽ���ģʽ
	USART_Init(USART2,&USART_InitStructure);	/* ��ʼ��USART1 */
	USART_Cmd(USART2, ENABLE);		   /* ʹ��USART1 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//ʹ�ܻ���ʧ��ָ����USART�ж� �����ж�
	USART_ClearFlag(USART2,USART_FLAG_TC);//���USARTx�Ĵ������־λ


	/* ����NVIC���� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 	   //��USART1��ȫ���ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	 //��ռ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 			//��Ӧ���ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			 //ʹ��
	NVIC_Init(&NVIC_InitStructure);	
}
void UsartSentData(USART_TypeDef* Usart,uint8_t Data)		
{
	USART_SendData(Usart,Data);
	while (USART_GetFlagStatus(Usart, USART_FLAG_TXE) == RESET);
}
void UsartSentDatas(USART_TypeDef* Usart,uint8_t *Data)		
{
	uint8_t i;
	for(i=0;Data[i]!='\0';i++)
		UsartSentData(Usart,Data[i]);
}
void USART1_IRQHandler()		//wsx:����1���͵�����3
{
	static uint8_t RXData;
	if (USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		RXData=USART_ReceiveData(USART1);
		UsartSentData(USART3,RXData);
	}
}

void USART2_IRQHandler()		//wsx:��������
{
 
	static uint8_t RXData;
	if (USART_GetITStatus(USART2,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);	
		RXData=USART_ReceiveData(USART2);
		//BTDataHandle(RXData);
//		UsartSentData(USART1,RXData);	
	}
}
void USART3_IRQHandler()		
{
	static uint8_t RXData;
	if (USART_GetITStatus(USART3,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);		
		RXData=USART_ReceiveData(USART3);
		//BC26DataHandle(RXData);
	}
}