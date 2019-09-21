#include "Usart2.h"

uint8_t ReceiveBufferTemp[3][RX_BUFFER_SIZE];
uint8_t ReceiveBuffer[3][RX_BUFFER_SIZE_X_4];
uint16_t ReceiveNum[3],ReadStart[3];
/*
    功能：初始化串口2 DMA通道
    返回值：无
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
	UsartInit.USART_BaudRate=115200;		//波特率
	UsartInit.USART_WordLength=USART_WordLength_8b;		//八位数据
	UsartInit.USART_StopBits=USART_StopBits_1;		//一位停止位
	UsartInit.USART_Parity=USART_Parity_No;		//无校验
	UsartInit.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//无硬件流控
	UsartInit.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;		//收发模式
	USART_Init(USART2,&UsartInit);	
	
	NVICUsart.NVIC_IRQChannel=USART2_IRQn;		//设置中断
	NVICUsart.NVIC_IRQChannelPreemptionPriority=0;	//设置中断优先级
	NVICUsart.NVIC_IRQChannelSubPriority=0;
	NVICUsart.NVIC_IRQChannelCmd=ENABLE;		//使能中断请求
	NVIC_Init(&NVICUsart);		
	
	USART_ClearFlag(USART2,USART_FLAG_TC);		//清除中断标志位
	USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);	
	USART_Cmd(USART2,ENABLE);		
	
	DMA_Cmd(DMA1_Channel6,DISABLE);
	DMA_DeInit(DMA1_Channel6);   //将DMA的通道5寄存器重设为缺省值  串口1对应的是DMA通道5
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);  
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&ReceiveBufferTemp[1][0]);  //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //数据传输方向，从外设读取发送到内存
    DMA_InitStructure.DMA_BufferSize = RX_BUFFER_SIZE;  //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //数据宽度为8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在正常缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMA通道 x拥有中优先级 
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
	DMA_Cmd(DMA1_Channel6, ENABLE); 
}

/*
    功能：串口2中断服务函数：将串口2收到的消息通过DMA通道直接写入flash
    返回值：无
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