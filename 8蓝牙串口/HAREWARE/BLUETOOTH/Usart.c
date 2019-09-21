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
	GPIO_InitTypeDef GPIO_InitStructure;	//声明一个结构体变量，用来初始化GPIO

	USART_InitTypeDef  USART_InitStructure;	  //串口结构体定义

	NVIC_InitTypeDef NVIC_InitStructure;//中断结构体定义
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	 //打开时钟
	
	/*  配置GPIO的模式和IO口 */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;//TX			   //串口输出PA9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;	    //复用推挽输出
	GPIO_Init(GPIOA,&GPIO_InitStructure);  /* 初始化串口输入IO */
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;//RX			 //串口输入PA10
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;		  //模拟输入
	GPIO_Init(GPIOA,&GPIO_InitStructure); /* 初始化GPIO */


	USART_InitStructure.USART_BaudRate=38400;   //波特率设置为9600	//波特率
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;		//数据长8位
	USART_InitStructure.USART_StopBits=USART_StopBits_1;			//1位停止位
	USART_InitStructure.USART_Parity=USART_Parity_No;				//无效验
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None; //失能硬件流
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;	 //开启发送和接受模式
	USART_Init(USART2,&USART_InitStructure);	/* 初始化USART1 */
	USART_Cmd(USART2, ENABLE);		   /* 使能USART1 */
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//使能或者失能指定的USART中断 接收中断
	USART_ClearFlag(USART2,USART_FLAG_TC);//清除USARTx的待处理标志位


	/* 设置NVIC参数 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);    
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; 	   //打开USART1的全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 	 //抢占优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 			//响应优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			 //使能
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
void USART1_IRQHandler()		//wsx:串口1发送到串口3
{
	static uint8_t RXData;
	if (USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET){
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		RXData=USART_ReceiveData(USART1);
		UsartSentData(USART3,RXData);
	}
}

void USART2_IRQHandler()		//wsx:蓝牙串口
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