#include "flash.h"
#include "Usart.h"
#include "BlueToothComm.h"
#include "DEBUG.h"

#define FLASH_SIZE 64          

#if FLASH_SIZE<256
  #define SECTOR_SIZE           1024    
#else 
  #define SECTOR_SIZE           2048    
#endif

#define STARTADDR 0x0800F000
#define ENDADDR 0x0800F3FF

volatile FLASH_Status FLASHStatus = FLASH_BUSY;
//const unsigned char DevicesDatas[] __attribute__((at(STARTADDR)))={"118.89.53.170\0""1883\0""ClientID001\0""Username\0""Password\0""LOCK-001\0"};
//const unsigned char DevicesDatas[] __attribute__((at(STARTADDR)))={"132.232.168.115\0""1883\0""ClientID001\0""client\0""client\0""BTname\0"};
uint8_t IPAddr[16]={"118.89.53.170"};
//uint8_t IPAddr[16]={"0.0.0.0"};
uint8_t Port[6]={"1883"};
//uint8_t Port[6]={"0000"};
uint8_t ClientID[17]={"ClientID001"};
uint8_t Username[17]={"Username"};
uint8_t Password[17]={"Password"};
uint8_t BTName[17]={"Wall-01"};
uint8_t HeartTimes[4]={"1"};
uint8_t AuthCode[7]={"000000"};
uint8_t DevicesID[25];
//uint8_t BTPassword[17];

void ReWriteDatas()//从FLASH读取数据
{
	uint32_t i;
    uint16_t Temp;
	uint32_t Address=STARTADDR;
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	FLASH_UnlockBank1();    
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
	while(FLASH_ErasePage(STARTADDR)!=FLASH_COMPLETE);
	for(i=0;(IPAddr[i]!='\0');i+=2){		//1	
		if((i!=0)&&(IPAddr[i-1]=='\0'))
			break;
		Temp=IPAddr[i]+((uint16_t)IPAddr[i+1]<<8);
		FLASH_ProgramHalfWord(Address,Temp);
		Address+=2;
	}
	if(IPAddr[i-1]!='\0'){
		Temp=((uint16_t)Port[0]<<8);
		FLASH_ProgramHalfWord(Address,Temp);
		Address+=2;
		i=1;
	}
	else
		i=0;
	
	for(;(Port[i]!='\0');i+=2){		//2
		if((i!=0)&&(Port[i-1]=='\0'))
			break;
		Temp=Port[i]+((uint16_t)Port[i+1]<<8);
		FLASH_ProgramHalfWord(Address,Temp);
		Address+=2;
	}
	if(Port[i-1]!='\0'){
		Temp=((uint16_t)ClientID[0]<<8);
		FLASH_ProgramHalfWord(Address,Temp);
		Address+=2;
		i=1;
	}
	else
		i=0;
	
	for(;(ClientID[i]!='\0');i+=2){		//3
		if((i!=0)&&(ClientID[i-1]=='\0'))
			break;
		Temp=ClientID[i]+((uint16_t)ClientID[i+1]<<8);
		FLASH_ProgramHalfWord(Address,Temp);
		Address+=2;
	}
	if(ClientID[i-1]!='\0'){
		Temp=((uint16_t)Username[0]<<8);
		FLASH_ProgramHalfWord(Address,Temp);
		Address+=2;
		i=1;
	}
	else
		i=0;
	
//	for(;(Username[i]!='\0');i+=2){		//4
//		if((i!=0)&&(Username[i-1]=='\0'))
//			break;
//		Temp=Username[i]+((uint16_t)Username[i+1]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//	}
//	if(Username[i-1]!='\0'){
//		Temp=((uint16_t)Password[0]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//		i=1;
//	}
//	else
//		i=0;
//	
//	for(;(Password[i]!='\0');i+=2){		//5
//		if((i!=0)&&(Password[i-1]=='\0'))
//			break;
//		Temp=Password[i]+((uint16_t)Password[i+1]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//	}
//	if(Password[i-1]!='\0'){
//		Temp=((uint16_t)BTName[0]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//		i=1;
//	}
//	else
//		i=0;
//	
//	for(;(BTName[i]!='\0');i+=2){		//6
//		if((i!=0)&&(BTName[i-1]=='\0'))
//			break;
//		Temp=BTName[i]+((uint16_t)BTName[i+1]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//	}
//	if(BTName[i-1]!='\0'){
//		Temp=((uint16_t)HeartTimes[0]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//		i=1;
//	}
//	else
//		i=0;
//	
//	for(;(HeartTimes[i]!='\0');i+=2){		//6
//		if((i!=0)&&(HeartTimes[i-1]=='\0'))
//			break;
//		Temp=HeartTimes[i]+((uint16_t)HeartTimes[i+1]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//	}
//	if(HeartTimes[i-1]!='\0'){
//		Temp=((uint16_t)AuthCode[0]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//		i=1;
//	}
//	else
//		i=0;
//	
//	for(;(AuthCode[i]!='\0');i+=2){		//6
//		if((i!=0)&&(AuthCode[i-1]=='\0'))
//			break;
//		Temp=AuthCode[i]+((uint16_t)AuthCode[i+1]<<8);
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//	}
//	if(AuthCode[i-1]!='\0'){
//		Temp=0X0000;
//		FLASH_ProgramHalfWord(Address,Temp);
//		Address+=2;
//		i=1;
//	}
	
	
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	FLASH_LockBank1(); 
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	
}
void SystemStartCheckGPIOInit(void)//B5端口初始化检测。检测是否连接蓝牙。
{
	GPIO_InitTypeDef GPIOInit;
	RCC_APB2PeriphClockCmd(SYSTEM_START_RCC,ENABLE);
	GPIOInit.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIOInit.GPIO_Pin=SYSTEM_START_PIN;//wsx：SYSTEM_START_PIN=GPIO_Pin_15
	GPIO_Init(SYSTEM_START_PORT,&GPIOInit);//wsx：SYSTEM_START_PORT=GPIOB
}
void GETDevicesID()
{
	uint32_t IDTemp[3];
	int8_t i,Temp;
	IDTemp[0]=*(uint32_t*)(0X1FFFF7E8);
	IDTemp[1]=*(uint32_t*)(0X1FFFF7EC);
	IDTemp[2]=*(uint32_t*)(0X1FFFF7F0);
	DevicesID[24]='\0';
	for(i=23;i>=16;i--){
		Temp=IDTemp[0]%16;//wsx：取出个位？
		if(Temp<10)
			DevicesID[i]=Temp+'0';
		else{
			Temp-=10;
			DevicesID[i]=Temp+'A';
		}
		IDTemp[0]>>=4;
	}
	for(;i>=8;i--){
		Temp=IDTemp[1]%16;
		if(Temp<10)
			DevicesID[i]=Temp+'0';
		else{
			Temp-=10;
			DevicesID[i]=Temp+'A';
		}
		IDTemp[1]>>=4;
	}
	for(;i>=0;i--){
		Temp=IDTemp[2]%16;
		if(Temp<10)
			DevicesID[i]=Temp+'0';
		else{
			Temp-=10;
			DevicesID[i]=Temp+'A';
		}
		IDTemp[2]>>=4;
	}
	UsartSentDatas(USART1,"ID:");
	UsartSentDatas(USART1,DevicesID);
	UsartSentDatas(USART1,"\r\n");
}
void InitDevicesDatas(void)//初始化，读取FLASH中的数据
{
	ReadFlashSeriesStrings(IPAddr,1);
	DEBUG(IPAddr);
	DEBUG("\r\n");
	ReadFlashSeriesStrings(Port,0);
	DEBUG(Port);
	DEBUG("\r\n");
	ReadFlashSeriesStrings(ClientID,0);
	DEBUG(ClientID);
	DEBUG("\r\n");
	GETDevicesID();
	for(uint8_t i=0;i<6;i++)
		SetAuth[i]=DevicesID[i];
	//ReadFlashSeriesStrings(BTPassword,0);
}
void ReadFlashString(uint8_t *Datas,uint8_t Index)
{
	uint32_t FlashAddress;
	uint8_t i;
	FlashAddress=(uint32_t)STARTADDR;
	while(--Index){
		while(((*((__IO uint8_t*)FlashAddress))!='\0')&&(FlashAddress<=ENDADDR))
			FlashAddress++;
		if((*((__IO uint8_t*)FlashAddress))=='\0')
			FlashAddress++;
	}
	for(i=0;(FlashAddress<=ENDADDR)&&((*((__IO uint8_t*)FlashAddress))!='\0');i++,FlashAddress++)
		Datas[i]=(*((__IO uint8_t*)FlashAddress));
	Datas[i]='\0';
}
void ReadFlashSeriesStrings(uint8_t *Datas,uint8_t Start)
{
	static uint32_t FlashAddress;
	uint8_t i;
	if(Start==1)
		FlashAddress=(uint32_t)STARTADDR;
	for(i=0;(FlashAddress<=ENDADDR)&&((*((__IO uint8_t*)FlashAddress))!='\0');i++,FlashAddress++)
		Datas[i]=(*((__IO uint8_t*)FlashAddress));
	FlashAddress++;
	Datas[i]='\0';
}
