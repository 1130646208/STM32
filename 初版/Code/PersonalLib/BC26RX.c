
#include "SystemInit.h"
#include "Usart.h"
#include "Timer.h"
#include "BC26RX.h"

void BC26UsartInit(void);
uint8_t RXARespondHandle(uint16_t Counts,uint16_t *Index,uint16_t *i);
uint8_t RXPRespondHandle(uint16_t Counts,uint16_t *Index,uint16_t *i);

void BC26RXPBufferReader(Task *TaskAddr);

BC26RXTypeDef BC26RX={.HardWareInit=BC26UsartInit,.PSegment=0,.PBufferReadPoint=0,.BC26RXPBufferReader=BC26RXPBufferReader};

void BC26UsartInit(void)
{
	USART_InitTypeDef UsartInit;
	NVIC_InitTypeDef NVICUsart;
	GPIO_InitTypeDef GPIOInit;
	DMA_InitTypeDef DMA_InitStructure;
	USART_DeInit(USART3);		//取消初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;	
	GPIOInit.GPIO_Pin=GPIO_Pin_10;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIOInit);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
	GPIOInit.GPIO_Pin=GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIOInit);
	
	USART_DeInit(USART3);
	UsartInit.USART_BaudRate=115200;		//波特率
	UsartInit.USART_WordLength=USART_WordLength_8b;		//八位数据
	UsartInit.USART_StopBits=USART_StopBits_1;		//一位停止位
	UsartInit.USART_Parity=USART_Parity_No;		//无校验
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//无硬件流控
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		//收发模式
	USART_Init(USART3,&UsartInit);	
	
	NVICUsart.NVIC_IRQChannel=USART3_IRQn;		//设置中断
	NVICUsart.NVIC_IRQChannelPreemptionPriority=0;	//设置中断优先级
	NVICUsart.NVIC_IRQChannelSubPriority=0;
	NVICUsart.NVIC_IRQChannelCmd=ENABLE;		//使能中断请求
	NVIC_Init(&NVICUsart);		
	
	USART_ClearFlag(USART3,USART_FLAG_TC);		//清除中断标志位
	USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);	
	USART_Cmd(USART3,ENABLE);		
	
	DMA_Cmd(DMA1_Channel3,DISABLE);
	DMA_DeInit(DMA1_Channel3);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&BC26RX.BufferTemp[0]);  //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
    DMA_InitStructure.DMA_BufferSize = BC26_RX_BUFFER_TEMP_SIZE;  //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //工作在正常缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道

	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE); 
}
void USART3_IRQHandler()		//中断函数
{
	static uint16_t Counts,IndexA=0,IndexP=0,i;
	uint8_t Ending=0;
	if(USART_GetITStatus(USART3,USART_IT_IDLE)!=RESET){
		DMA_Cmd(DMA1_Channel3,DISABLE);
		USART_ReceiveData(USART3);
		Counts=BC26_RX_BUFFER_TEMP_SIZE-DMA1_Channel3->CNDTR;
		for(i=0;(BC26RX.BufferTemp[i]=='\r')||(BC26RX.BufferTemp[i]=='\n')||(BC26RX.BufferTemp[i]=='\0');i++);
		if((Counts!=BC26_RX_BUFFER_TEMP_SIZE)&&(Counts!=i)&&((BC26RX.BufferTemp[i]=='A')||(BC26RX.BufferTemp[i]=='+')||(BC26RX.ATCOMStatus==BC26_ATCOM_ANALYSIS))){
			do{
				if(BC26RX.ATCOMStatus==BC26_ATCOM_ANALYSIS){
					if(BC26RX.BufferTemp[i]=='+')
						Ending=0;
					Ending=RXARespondHandle(Counts,&IndexA,&i);
				}
				else if(BC26RX.BufferTemp[i]=='A'){
					IndexA=0;
					BC26RX.ATCOMStatus=BC26_ATCOM_ANALYSIS;
					Ending=RXARespondHandle(Counts,&IndexA,&i);
				}
				else if(BC26RX.BufferTemp[i]=='+'){
					Ending=RXPRespondHandle(Counts,&IndexP,&i);
				}
			}while(Ending==0);
		}
		DMA_SetCurrDataCounter(DMA1_Channel3,BC26_RX_BUFFER_TEMP_SIZE);
		DMA_Cmd(DMA1_Channel3,ENABLE);
		USART_ClearITPendingBit(USART3,USART_IT_IDLE);
	}
}
uint8_t RXARespondHandle(uint16_t Counts,uint16_t *Index,uint16_t *i)
{
	uint8_t MeetNR=0;
	if((BC26RX.BufferTemp[(*i)]=='O')||(BC26RX.BufferTemp[(*i)]=='E'))
		MeetNR=1;
	for(;(*i)<Counts;(*i)++){
		if((BC26RX.BufferTemp[(*i)]=='\r')||(BC26RX.BufferTemp[(*i)]=='\n')){
			if(MeetNR==0){
				MeetNR=1;
				BC26RX.ABuffer[(*Index)]='\n';
				(*Index)++;
				if((*Index)==BC26_RX_ABUFFER_SIZE)
					(*Index)=0;
			}
		}
		else{
			if((BC26RX.ATCOMStatus==BC26_ATCOM_RESPOND)&&(MeetNR==1))
				return 0;
			if(MeetNR==1){
				if(BC26RX.BufferTemp[(*i)]=='E'){
					BC26RX.ATCOMResult=BC26_ATCOM_ERROR;
					BC26RX.ATCOMStatus=BC26_ATCOM_RESPOND;
				}
				else if(BC26RX.BufferTemp[(*i)]=='O'){
					BC26RX.ATCOMResult=BC26_ATCOM_OK;
					BC26RX.ATCOMStatus=BC26_ATCOM_RESPOND;
				}
			}
			MeetNR=0;
			BC26RX.ABuffer[(*Index)]=BC26RX.BufferTemp[(*i)];
			(*Index)++;
			if((*Index)==BC26_RX_ABUFFER_SIZE)
				(*Index)=0;
		}
	}
	if(BC26RX.ATCOMStatus==BC26_ATCOM_RESPOND){
		BC26RX.ABuffer[(*Index)]='\0';
		(*Index)++;
		if((*Index)==BC26_RX_ABUFFER_SIZE)
			(*Index)=0;
	}
	return 1;	
}
uint8_t RXPRespondHandle(uint16_t Counts,uint16_t *Index,uint16_t *i)
{
	uint8_t MeetNR=0;
	for(;(*i)<Counts;(*i)++){
		if((BC26RX.BufferTemp[(*i)]=='\r')||(BC26RX.BufferTemp[(*i)]=='\n')){
			if(MeetNR==0){
				MeetNR=1;
				BC26RX.PBuffer[(*Index)]='\n';
				BC26RX.PSegment++;
				(*Index)++;
				if((*Index)==BC26_RX_PBUFFER_SIZE)
					(*Index)=0;
			}
		}
		else{
			if(MeetNR==1)
				return 0;
			MeetNR=0;
			BC26RX.PBuffer[(*Index)]=BC26RX.BufferTemp[(*i)];
			(*Index)++;
			if((*Index)==BC26_RX_PBUFFER_SIZE)
				(*Index)=0;
		}
	}
	return 1;	
}
void BC26RXPBufferReader(Task *TaskAddr)
{
	uint16_t i=0;
	for(;BC26RX.PSegment>0;BC26RX.PSegment--){
		do{
			StringTemp[i]=BC26RX.PBuffer[BC26RX.PBufferReadPoint];
			i++;
			BC26RX.PBufferReadPoint++;
			if(BC26RX.PBufferReadPoint==BC26_RX_PBUFFER_SIZE)
				BC26RX.PBufferReadPoint=0;
		}while(BC26RX.PBuffer[BC26RX.PBufferReadPoint]!='\n');
		BC26RX.PBufferReadPoint++;
		if(BC26RX.PBufferReadPoint==BC26_RX_PBUFFER_SIZE)
			BC26RX.PBufferReadPoint=0;
		StringTemp[i]='\n';
		i++;
		StringTemp[i]='\0';
		DEBUG(StringTemp);
		if(strstr(StringTemp,"+QMTRECV: 0,0")==StringTemp)
			;
		else if(strstr(StringTemp,"+QMTPUB: 0,1,0")==StringTemp)
			BC26PubOK=1;
		else if(strstr(StringTemp,"+QMTUNS: 0,1,0")==StringTemp)
			BC26UnsubOK=1;
		else if(strstr(StringTemp,"+QMTSUB: 0,1,0")==StringTemp)
			BC26SubOK=1;
		else if(strstr(StringTemp,"+QMTOPEN: 0,0")==StringTemp)
			BC26SuccessOpen=1;
		else if(strstr(StringTemp,"+QMTCONN: 0,0,0")==StringTemp)
			BC26SuccessConnect=1;
	}
}


//void BC26RXHandle(Task *TaskAddr)
//{
//	static Timer TestTimer={.Using=0};
//	if(BC26RXStatus.Segment>0){
//		TimerSub(&TestTimer);
//		do{
//			//read;
//			BC26RXStatus.BufferReadPoint++;
//			if(BC26RXStatus.BufferReadPoint==BC26_RX_BUFFER_SIZE)
//				BC26RXStatus.BufferReadPoint=0;
//		}while(BC26RXStatus.Buffer[BC26RXStatus.BufferReadPoint]!='\n');
//		BC26RXStatus.BufferReadPoint++;
//		if(BC26RXStatus.BufferReadPoint==BC26_RX_BUFFER_SIZE)
//			BC26RXStatus.BufferReadPoint=0;
//		BC26RXStatus.Segment--;
//	}
//	else {
//		TimerAdd(&TestTimer,5000,1);
//		if(TimerLoopTimeOut(&TestTimer)==TIMER_TIMEOUT){
//			TimerSub(&TestTimer);
//			UsartSentDatas(USART3,"ENTER SLEEP\r\n");
//			UsartSentDatas(USART3,"Leave SLEEP\r\n");
//		}
//	}
//}

//			MeetNR=0;
//			for(i=0;i<Counts;i++){
//				if((BC26RXStatus.BufferTemp[i]=='\r')||(BC26RXStatus.BufferTemp[i]=='\n')){
//					if(MeetNR==0){
//						MeetNR=1;
//						AimBuffer[(*AimIndex)]='\n';
//						(*AimSegment)++;
//						(*AimIndex)++;
//						if((*AimIndex)==BC26_RX_BUFFER_SIZE)
//							(*AimIndex)=0;
//					}
//				}
//				else{
//					MeetNR=0;
//					AimBuffer[(*AimIndex)]=BC26RXStatus.BufferTemp[i];
//					(*AimIndex)++;
//					if((*AimIndex)==BC26_RX_BUFFER_SIZE)
//						(*AimIndex)=0;
//				}
//			}
