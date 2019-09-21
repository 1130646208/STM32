#include "stm32f10x_usart.h"
#include "Usart.h"
#include "bluetooth.h"
#include "delay.h"
uint8_t BTPassword[17]={"123456"};
uint8_t BTName[17]={"WSX"};

void BTInit(void);


void BTGPIOInit(void)//wsx: ATÃüÁî¿Ú PA6
{
	GPIO_InitTypeDef GPIOInit;
	RCC_APB2PeriphClockCmd(BLUETOOTH_PORT_RCC,ENABLE);
	GPIOInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIOInit.GPIO_Pin=BLUETOOTH_AT_PIN;
	GPIO_Init(BLUETOOTH_PORT,&GPIOInit);
    UsartSentDatas(USART1,"BT gpio init ok.\r\n");
    BTInit();
}
uint8_t IsOK=0;
void BTInit(void)
{
	BLUETOOTH_ENTER_AT;
	delay_ms(500);
	UsartSentDatas(USART2,"AT+INIT\r\n");
	while(IsOK==0);
	IsOK=0;
	UsartSentDatas(USART2,"AT+UART=38400,0,0\r\n");
	while(IsOK==0);
	IsOK=0;
	UsartSentDatas(USART2,"AT+RMAAD\r\n");
	while(IsOK==0);
	IsOK=0;
	//GenerateBTPassword();
	UsartSentDatas(USART2,"AT+PSWD=\"");
	UsartSentDatas(USART2,BTPassword);
	UsartSentDatas(USART2,"\"\r\n");
	while(IsOK==0);
	IsOK=0;
	UsartSentDatas(USART2,"AT+NAME=\"");
	UsartSentDatas(USART2,BTName);
	UsartSentDatas(USART2,"\"\r\n");
	while(IsOK==0);
	IsOK=0;
	BLUETOOTH_LEAVE_AT;
	UsartSentDatas(USART2,"AT+RESET\r\n");
	while(IsOK==0);
	IsOK=0;
	UsartSentDatas(USART1,"BT INITED\r\n");
}

