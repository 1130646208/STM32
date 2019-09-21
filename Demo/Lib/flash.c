#include "flash.h"
#include "Usart.h"
#include "CharNum.h"
#include "DEBUG.h"

#define STARTADDR 0x0800F000
#define ENDADDR 0x0800F3FF

uint8_t IPAddr[16]={"47.106.249.19"};
uint8_t Port[6]={"61613"};
uint8_t ClientID[17]={"default"};
uint8_t Username[17]={"admin"};
uint8_t Password[17]={"password"};
uint8_t DevicesID[25];

void WriteDatas(uint8_t *Datas1,uint8_t *Datas2,uint32_t *i,uint32_t *Address,uint8_t IsEnd);

void ReadFlashSeriesStrings(uint8_t *Datas,uint8_t Start);

void WriteDatas(uint8_t *Datas1,uint8_t *Datas2,uint32_t *i,uint32_t *Address,uint8_t IsEnd)
{
	uint16_t Temp;
	for(;(Datas1[(*i)]!='\0');(*i)+=2){		
		if(((*i)!=0)&&(Datas1[(*i)-1]=='\0'))
			break;
		Temp=Datas1[(*i)]+((uint16_t)Datas1[(*i)+1]<<8);
		FLASH_ProgramHalfWord((*Address),Temp);
		(*Address)+=2;
	}
	
	if(Datas1[(*i)-1]!='\0'){
		if(IsEnd==1)
			Temp=0X0000;
		else
		 Temp=((uint16_t)Datas2[0]<<8);
		FLASH_ProgramHalfWord((*Address),Temp);
		(*Address)+=2;
		(*i)=1;
	}
	else
		(*i)=0;
}
void ReWriteDatas()
{
	uint32_t i=0;
	uint32_t Address=STARTADDR;
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	FLASH_UnlockBank1();    
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
  FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
	while(FLASH_ErasePage(STARTADDR)!=FLASH_COMPLETE);
	WriteDatas(IPAddr,Port,&i,&Address,0);
	WriteDatas(Port,ClientID,&i,&Address,0);
	WriteDatas(ClientID,Username,&i,&Address,0);
	WriteDatas(Username,Password,&i,&Address,0);
	WriteDatas(Password,Password,&i,&Address,1);
	
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	FLASH_LockBank1(); 
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	
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
		Temp=IDTemp[0]%16;
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
}
void InitDevicesDatas(void)
{
	ReadFlashSeriesStrings(IPAddr,1);
	ReadFlashSeriesStrings(Port,0);
	ReadFlashSeriesStrings(ClientID,0);
	ReadFlashSeriesStrings(Username,0);
	ReadFlashSeriesStrings(Password,0);
	GETDevicesID();
	DEBUG("IPAddr:");
	DEBUG(IPAddr);
	DEBUG("\r\n");
	DEBUG("Port:");
	DEBUG(Port);
	DEBUG("\r\n");
	DEBUG("ClientID:");
	DEBUG(ClientID);
	DEBUG("\r\n");
	DEBUG("Username:");
	DEBUG(Username);
	DEBUG("\r\n");
	DEBUG("Password:");
	DEBUG(Password);
	DEBUG("\r\n");
	DEBUG("DevicesID:");
	DEBUG(DevicesID);
	DEBUG("\r\n");
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
uint8_t StringEqual(uint8_t *String1,uint8_t *String2)
{
	uint16_t Length1,Length2;
	uint16_t i;
	Length1=StringLength(String1);
	Length2=StringLength(String2);
	if(Length2==Length1){
		for(i=0;i<Length1;i++){
			if(String1[i]==String2[i])
				continue;
			else
				return 0;
		}
	}
	else
		return 0;
	return 1;
}
uint16_t StringLength(uint8_t *String)
{
	uint16_t i;
	for(i=0;String[i]!='\0';i++);
	return i;
}
