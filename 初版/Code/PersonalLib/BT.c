#include "BT.h"
#include "Delay.h"
#include "Usart.h"
#include "flash.h"
#include "DEBUG.h"

void BTInit(void);
uint8_t BTRxOK;

void BTGPIOInit(void)
{
	GPIO_InitTypeDef GPIOInit;
	RCC_APB2PeriphClockCmd(BLUETOOTH_PORT_RCC,ENABLE);
	GPIOInit.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIOInit.GPIO_Speed=GPIO_Speed_50MHz;
	GPIOInit.GPIO_Pin=BLUETOOTH_AT_PIN;
	GPIO_Init(BLUETOOTH_PORT,&GPIOInit);
	BTInit();
}
void BTInit(void)
{
	BLUETOOTH_ENTER_AT;
	DelayMs(500);
	UsartSentDatas(USART2,"AT+INIT\r\n");
	while(BTRxOK==0);
	BTRxOK=0;
	DEBUG("BT INIT.\r\n");
	UsartSentDatas(USART2,"AT+UART=38400,0,0\r\n");
	while(BTRxOK==0);
	BTRxOK=0;
	DEBUG("BT UART.\r\n");
	UsartSentDatas(USART2,"AT+RMAAD\r\n");
	while(BTRxOK==0);
	BTRxOK=0;
	DEBUG("BT RMAAD.\r\n");
	UsartSentDatas(USART2,"AT+PSWD=\"");
	UsartSentDatas(USART2,BTPassword);
	UsartSentDatas(USART2,"\"\r\n");
	while(BTRxOK==0);
	BTRxOK=0;
	DEBUG("BT PSWD.\r\n");
	UsartSentDatas(USART2,"AT+NAME=\"");
	UsartSentDatas(USART2,DevicesID);
	UsartSentDatas(USART2,"\"\r\n");
	while(BTRxOK==0);
	BTRxOK=0;
	DEBUG("BT NAME.\r\n");
	UsartSentDatas(USART2,"AT+RESET\r\n");
	while(BTRxOK==0);
	BTRxOK=0;
	DEBUG("BT RESET.\r\n");
	BLUETOOTH_LEAVE_AT;
}
uint8_t SetBTPasswd(uint8_t *Receives)
{
	uint8_t i,j;
	uint8_t Flag=1;
	for(i=0,j=0;j!=3;i++)
		if(Receives[i]=='\"')
			j++;
	Receives+=i;
	for(i=0;BTPassword[i]!='\0';i++){
		if(BTPassword[i]!=Receives[i]){
			Flag=0;
			break;
		}
	}
	if((Flag==1)&&(Receives[i]!='\"'))
		Flag=0;
	if(Flag==1)
		return 0;
	for(i=0;(Receives[i]!='\"')&&(i<=16);i++)
		BTPassword[i]=Receives[i];
	BTPassword[i]='\0';
	ReWriteDatas();
	InitDevicesDatas();//!!!!!!!!!!!!!!!!!!!!!!
	if(Receives[i]=='\"'){
		BLUETOOTH_ENTER_AT;
		DelayMs(300);
		UsartSentDatas(USART2,"AT+RMAAD\r\n");
		while(BTRxOK==0);
		BTRxOK=0;
		UsartSentDatas(USART2,"AT+PSWD=\"");
		UsartSentDatas(USART2,BTPassword);
		UsartSentDatas(USART2,"\"\r\n");
		while(BTRxOK==0);
		BTRxOK=0;
		UsartSentDatas(USART2,"AT+RESET\r\n");
		while(BTRxOK==0);
		BTRxOK=0;
		BLUETOOTH_LEAVE_AT;
		return 0;
	}
	else
		return 1;
}