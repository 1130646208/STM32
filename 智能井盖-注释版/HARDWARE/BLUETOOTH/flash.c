#include "flash.h"
#include "Delay.h"
#define STARTADDR 0x0800F000
#define ENDADDR 0x0800F3FF

void WriteDatas(uint8_t *Datas1,uint8_t *Datas2,uint32_t *i,uint32_t *Address,uint8_t IsEnd);
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
	WriteDatas((uint8_t *)Port,(uint8_t *)Port,&i,&Address,1);
    //DEBUG0("ReWriteDatas");
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	FLASH_LockBank1(); 
	while(FLASH_GetStatus()!=FLASH_COMPLETE);
	
}
/*
    功能：将IP和Port从flash读取到内存
    返回值：无
*/
void InitDevicesDatas(void)
{
    char temp[20]="IP:";
    char temp2[20]="Port:";
	ReadFlashSeriesStrings((uint8_t *)IPAddr,1);
	ReadFlashSeriesStrings((uint8_t *)Port,0);
	strcat(temp,IPAddr);DEBUG0(temp);//DEBUG("\r\n");
	strcat(temp2,Port);DEBUG0(temp2);//DEBUG("\r\n");

}
/*
    功能：重写flash信息
    返回值：无
*/
void FirstRunning(void)
{
	//if((*((__IO uint8_t*)0x0800F000))==0xff){
		DEBUG0("Rewriting......");
		ReWriteDatas();		
	//}
}
/*
    功能：读取flash信息
    返回值：无
*/
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

/*
    功能：写入flash信息
    返回值：无
*/
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
/*
    功能：蓝牙设置井盖ID
          若收到第一个字母不是I/i则清除信息
    返回值：无
*/
u8 SetNetInfo()
{
    u8 i;
    DelayMs(1000);
    DEBUG0("SET ID.\r\n");DEBUG(ReceiveBufferTemp[1]);
    if(ReceiveBufferTemp[1][0]=='I'||ReceiveBufferTemp[1][0]=='i'){
        for(i=1;ReceiveBufferTemp[1][i]!='\0';i++){
            ID[i-1]=ReceiveBufferTemp[1][i];
        }
        DEBUG0(ID);
        return 1;
    }else{
        DEBUG0("ID NOT SET.\r\n");
        ClearNetInfo();
        SetNetInfo();
    }
}
/*
    功能：清除ID信息
    返回值：无
*/
void ClearNetInfo()
{ 
    u8 i;
    for (i=0;i<sizeof(ID);i++)
    {
        ID[i]='\0';
    }
}    
