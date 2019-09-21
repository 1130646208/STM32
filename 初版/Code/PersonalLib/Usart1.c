#include "Usart.h"
#include "delay.h"
#include "math.h"
#include "Task.h"
#include "Timer.h"
#include "stdio.h"
#include "LowPower.h"

extern uint8_t ReceiveBufferTemp[3][RX_BUFFER_SIZE];
extern uint8_t ReceiveBuffer[3][RX_BUFFER_SIZE_X_4];
extern uint16_t ReceiveNum[3],ReadStart[3];

extern CircularQueue *StartTask;
Task UsartRXTask;

void Usart1RX(Task *TaskAddr);

void Usart1Init(void)
{
	USART_InitTypeDef UsartInit;
	NVIC_InitTypeDef NVICUsart;
	GPIO_InitTypeDef GPIOInit;
	DMA_InitTypeDef DMA_InitStructure;
	USART_DeInit(USART1);		//取消初始化
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA,ENABLE);	//打开时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_AF_PP;	//TX脚
	GPIOInit.GPIO_Pin=GPIO_Pin_9;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOInit);
	
	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;	//RX脚
	GPIOInit.GPIO_Pin=GPIO_Pin_10;
	GPIO_Init(GPIOA,&GPIOInit);
	
	USART_DeInit(USART1);
	UsartInit.USART_BaudRate=115200;		//波特率
	UsartInit.USART_WordLength=USART_WordLength_8b;		//八位数据
	UsartInit.USART_StopBits=USART_StopBits_1;		//一位停止位
	UsartInit.USART_Parity=USART_Parity_No;		//无校验
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//无硬件流控
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		//收发模式
	USART_Init(USART1,&UsartInit);	
	
	NVICUsart.NVIC_IRQChannel=USART1_IRQn;		//设置中断
	NVICUsart.NVIC_IRQChannelPreemptionPriority=0;	//设置中断优先级
	NVICUsart.NVIC_IRQChannelSubPriority=0;
	NVICUsart.NVIC_IRQChannelCmd=ENABLE;		//使能中断请求
	NVIC_Init(&NVICUsart);		
	
	USART_ClearFlag(USART1,USART_FLAG_TC);		//清除中断标志位
	USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);	
	USART_Cmd(USART1,ENABLE);		
	
	DMA_Cmd(DMA1_Channel5,DISABLE);
	DMA_DeInit(DMA1_Channel5);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);  
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&ReceiveBufferTemp[0][0]);  //DMA内存基地址
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
  DMA_InitStructure.DMA_BufferSize = RX_BUFFER_SIZE;  //DMA通道的DMA缓存的大小
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在正常缓存模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
  DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道

	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE); 
}
void USART1_IRQHandler()		//中断函数
{
	static uint16_t Counts,Index=0,i;
	static uint8_t MeetNR;
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET){
		DMA_Cmd(DMA1_Channel5,DISABLE);
		USART_ReceiveData(USART1);
		Counts=RX_BUFFER_SIZE-DMA1_Channel5->CNDTR;
		if((Index+Counts)>=RX_BUFFER_SIZE_X_4){
			ReceiveBuffer[0][Index]='\0';
			Index=0;
		}
		MeetNR=0;
		for(i=0;i<Counts;i++){
			if((ReceiveBufferTemp[0][i]=='\r')||(ReceiveBufferTemp[0][i]=='\n')){
				if(MeetNR==0){
					MeetNR=1;
					ReceiveBuffer[0][Index]='\n';
					Index++;
				}
			}
			else{
				MeetNR=0;
				ReceiveBuffer[0][Index]=ReceiveBufferTemp[0][i];
				Index++;
			}
		}
		ReceiveBuffer[0][Index]='\0';
		if(Index==RX_BUFFER_SIZE_X_4-1)
			Index=0;
		else
			Index++;
		ReceiveNum[0]++;
		DMA_SetCurrDataCounter(DMA1_Channel5,RX_BUFFER_SIZE);
		DMA_Cmd(DMA1_Channel5,ENABLE);
		USART_ClearITPendingBit(USART1,USART_IT_IDLE);
	}
}
//void Usart1RX(Task *TaskAddr)
//{
//	static Timer TestTimer={.Using=0};
//	if(ReceiveNum[0]>0){
//		TimerSub(&TestTimer);
//		if(ReceiveBuffer[0][ReadStart[0]]=='\0')
//			ReadStart[0]=0;
//		SentDatas(USART1,(char*)(ReceiveBuffer[0]+ReadStart[0]));
//		do{
//			//read;
//			ReadStart[0]++;
//		}while(ReceiveBuffer[0][ReadStart[0]]!='\0');
//		if(ReadStart[0]==(RX_BUFFER_SIZE_X_4-1))
//			ReadStart[0]=0;
//		else
//			ReadStart[0]++;
//		ReceiveNum[0]--;
//	}
//	else {
//		TimerAdd(&TestTimer,5000,1);
//		if(TimerLoopTimeOut(&TestTimer)==TIMER_TIMEOUT){
//			TimerSub(&TestTimer);
//			SentDatas(USART1,"ENTER SLEEP\r\n");
//			SystemEnterSleep();
//			SentDatas(USART1,"Leave SLEEP\r\n");
//		}
//	}
//}

