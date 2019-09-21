#include "stm32f10x.h"

//����ͨ��ʵ��



void My_USART_Init(void){
    
    
    GPIO_InitTypeDef GPIO_InitStru;
    USART_InitTypeDef USART_InitStru;
    NVIC_InitTypeDef NVIC_InitStru;  
    /*�ٶ˿�ʱ��ʹ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//����ǰʹ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//���ú�ʹ��
    
    /*������GPIOģʽ���� PA9 PA10*/
    
    //TX����
    GPIO_InitStru.GPIO_Mode=GPIO_Mode_AF_PP;//���츴�����
    GPIO_InitStru.GPIO_Pin=GPIO_Pin_9;   //TX  PA9
    GPIO_InitStru.GPIO_Speed=GPIO_Speed_10MHz;

    GPIO_Init(GPIOA,&GPIO_InitStru);
    //RX����
    GPIO_InitStru.GPIO_Mode=GPIO_Mode_IN_FLOATING;//���� STM32�ֲ� P110
    GPIO_InitStru.GPIO_Pin=GPIO_Pin_10;   //RX  PA10
    GPIO_InitStru.GPIO_Speed=GPIO_Speed_10MHz;
    
    GPIO_Init(GPIOA,&GPIO_InitStru);
    
    
    /*�ܴ��ڲ�����ʼ��*/
    
    USART_InitStru.USART_BaudRate=115200;
    USART_InitStru.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
    USART_InitStru.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;//*******���˴�����ʵ��
    USART_InitStru.USART_Parity=USART_Parity_No;
    USART_InitStru.USART_StopBits=USART_StopBits_1;
    USART_InitStru.USART_WordLength=USART_WordLength_8b;
    
    USART_Init(USART1,&USART_InitStru);
    USART_Cmd(USART1,ENABLE);//ʹ�ܴ���1/*��ʹ�ܴ���1*/
    
    
    
    /*�ݴ����ж�����*/

    
    NVIC_InitStru.NVIC_IRQChannel=USART1_IRQn;
    NVIC_InitStru.NVIC_IRQChannelCmd=ENABLE;
    NVIC_InitStru.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
    NVIC_InitStru.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
    
    NVIC_Init(&NVIC_InitStru);
    USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//*****���˲���ʵ��
    /**
  * @brief  �������߹ر��ж�
  * @param  USARTx: ѡ��USART����UART���� 
  *   This parameter can be one of the following values:
  *   USART1, USART2, USART3, UART4 or UART5.
  * @param  USART_IT: specifies the USART interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *     @arg USART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
  *     @arg USART_IT_LBD:  LIN Break detection interrupt
  *     @arg USART_IT_TXE:  ���ͼĴ������ж�
  *     @arg USART_IT_TC:   Transmission complete interrupt
  *     @arg USART_IT_RXNE: ���ռĴ����ǿ��ж�
  *     @arg USART_IT_IDLE: Idle line detection interrupt
  *     @arg USART_IT_PE:   Parity Error interrupt
  *     @arg USART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @param  NewState: new state of the specified USARTx interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */

}

/*��д�����жϴ�����*/
void USART1_IRQHandler(void){

    
    u8 temp;
    if(USART_GetFlagStatus(USART1,USART_IT_RXNE)){
    /*�മ�������շ�*/
        temp=USART_ReceiveData(USART1);
        USART_SendData(USART1,temp);
    }
}

int main(void){

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����жϷ���2
    My_USART_Init();
    
    while(1); //�ȴ��ж�  
}    














