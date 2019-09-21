#include "stm32f10x.h"

//串口通信实验



void My_USART_Init(void){
    
    
    GPIO_InitTypeDef GPIO_InitStru;
    USART_InitTypeDef USART_InitStru;
    NVIC_InitTypeDef NVIC_InitStru;  
    /*①端口时钟使能*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//复用前使能
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//复用后使能
    
    /*③两个GPIO模式配置 PA9 PA10*/
    
    //TX配置
    GPIO_InitStru.GPIO_Mode=GPIO_Mode_AF_PP;//推挽复用输出
    GPIO_InitStru.GPIO_Pin=GPIO_Pin_9;   //TX  PA9
    GPIO_InitStru.GPIO_Speed=GPIO_Speed_10MHz;

    GPIO_Init(GPIOA,&GPIO_InitStru);
    //RX配置
    GPIO_InitStru.GPIO_Mode=GPIO_Mode_IN_FLOATING;//浮空 STM32手册 P110
    GPIO_InitStru.GPIO_Pin=GPIO_Pin_10;   //RX  PA10
    GPIO_InitStru.GPIO_Speed=GPIO_Speed_10MHz;
    
    GPIO_Init(GPIOA,&GPIO_InitStru);
    
    
    /*④串口参数初始化*/
    
    USART_InitStru.USART_BaudRate=115200;
    USART_InitStru.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStru.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;//*******出了错误不能实现
    USART_InitStru.USART_Parity=USART_Parity_No;
    USART_InitStru.USART_StopBits=USART_StopBits_1;
    USART_InitStru.USART_WordLength=USART_WordLength_8b;
    
    USART_Init(USART1,&USART_InitStru);
    USART_Cmd(USART1,ENABLE);//使能串口1/*⑥使能串口1*/
    
    
    
    /*⑤串口中断配置*/

    
    NVIC_InitStru.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
    NVIC_InitStru.NVIC_IRQChannelSubPriority=1;//响应优先级
    
    NVIC_Init(&NVIC_InitStru);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//*****丢了不能实现
    /**
  * @brief  开启或者关闭中断
  * @param  USARTx: 选择USART或者UART外设 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3, UART4 or UART5.
  * @param  USART_IT: specifies the USART interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
  *     @arg USART_IT_LBD:  LIN Break detection interrupt
  *     @arg USART_IT_TXE:  发送寄存器空中断
  *     @arg USART_IT_TC:   Transmission complete interrupt
  *     @arg USART_IT_RXNE: 接收寄存器非空中断
  *     @arg USART_IT_IDLE: Idle line detection interrupt
  *     @arg USART_IT_PE:   Parity Error interrupt
  *     @arg USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @param  NewState: new state of the specified USARTx interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

}

/*编写串口中断处理函数*/
void USART1_IRQHandler(void){

    
    u8 temp;
    if(USART_GetFlagStatus(USART1,USART_IT_RXNE)){
    /*⑧串口数据收发*/
        temp=USART_ReceiveData(USART1);
        USART_SendData(USART1,temp);
    }
}

int main(void){

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断分组2
    My_USART_Init();
    
    while(1); //等待中断  
}    














