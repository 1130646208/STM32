#include "flash.h"
#include "Usart.h"
#include "DEBUG.h"
#include "Lock.h"
#include "BC26Task.h"

#define STARTADDR 0x0800F000
#define ENDADDR 0x0800F3FF

uint8_t DevicesID[25];

void WriteDatas(uint8_t *Datas1,uint8_t *Datas2,uint32_t *i,uint32_t *Address,uint8_t IsEnd);
void GETDevicesID(void);
void ReadFlashSeriesStrings(uint8_t *Datas,uint8_t Start);


void ReWriteDatas()
{
	uint32_t i=0;
	uint32_t Address=STARTADDR;
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	FLASH_UnlockBank1();    
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR); 
	while(FLASH_ErasePage(STARTADDR)!=FLASH_COMPLETE);
	WriteDatas((uint8_t *)IPAddr,(uint8_t *)Port,&i,&Address,0);
	WriteDatas((uint8_t *)Port,(uint8_t *)ClientID,&i,&Address,0);
	WriteDatas((uint8_t *)ClientID,(uint8_t *)UserName,&i,&Address,0);
	WriteDatas((uint8_t *)UserName,(uint8_t *)Password,&i,&Address,0);
	WriteDatas((uint8_t *)Password,(uint8_t *)Status,&i,&Address,0);
	WriteDatas((uint8_t *)Status,(uint8_t *)HeartTimes,&i,&Address,0);
	WriteDatas((uint8_t *)HeartTimes,(uint8_t *)AuthCode,&i,&Address,0);
	WriteDatas((uint8_t *)AuthCode,(uint8_t *)BTPassword,&i,&Address,0);
	WriteDatas((uint8_t *)BTPassword,(uint8_t *)BTPassword,&i,&Address,1);
	
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	FLASH_LockBank1(); 
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	
}
void GETDevicesID(void)
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
	ReadFlashSeriesStrings((uint8_t *)IPAddr,1);
	ReadFlashSeriesStrings((uint8_t *)Port,0);
	ReadFlashSeriesStrings((uint8_t *)ClientID,0);
	ReadFlashSeriesStrings((uint8_t *)UserName,0);
	ReadFlashSeriesStrings((uint8_t *)Password,0);
	ReadFlashSeriesStrings((uint8_t *)Status,0);
	ReadFlashSeriesStrings((uint8_t *)HeartTimes,0);
	ReadFlashSeriesStrings((uint8_t *)AuthCode,0);
	ReadFlashSeriesStrings((uint8_t *)BTPassword,0);
	GETDevicesID();
	DEBUG("IP:");DEBUG(IPAddr);DEBUG("\r\n");
	DEBUG("PORT£º");DEBUG(Port);DEBUG("\r\n");
	DEBUG("ClientID£º");DEBUG(ClientID);DEBUG("\r\n");
	DEBUG("UserName:");DEBUG(UserName);DEBUG("\r\n");
	DEBUG("Password:");DEBUG(Password);DEBUG("\r\n");
	DEBUG("Status:");DEBUG(Status);DEBUG("\r\n");
	DEBUG("HeartTimes:");DEBUG(HeartTimes);DEBUG("\r\n");
	DEBUG("AuthCode:");DEBUG(AuthCode);DEBUG("\r\n");
	DEBUG("BTPassword:");DEBUG(BTPassword);DEBUG("\r\n");
	DEBUG("DevicesID:");DEBUG(DevicesID);DEBUG("\r\n");
}
void FirstRunning(void)
{
	//if((*((__IO uint8_t*)0x0800F000))==0xff){
		DEBUG("REWRITE\r\n");
		ReWriteDatas();		
	//}
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
