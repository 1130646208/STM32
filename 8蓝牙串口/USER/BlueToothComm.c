#include "BlueToothComm.h"
#include "flash.h"
//#include "Lock.h"
#include "DEBUG.H"
#include "BlueTooth.h"
#include "Usart.h"
#define START 1
#define END 0

#define ERROR 1
#define NO_ERROR 0


uint8_t BTCommStartFlag=END;

uint8_t SetAuth[7];


uint8_t BTBuffer[BTMAXROW][BTMAXBUFFER];
uint8_t BTLength[BTMAXROW];
uint8_t BTError[BTMAXROW];
int8_t BTRow=0;

extern uint8_t IsOK;

void BTSet(uint8_t *Datas);
void BTOpen(uint8_t *Datas);

 void BTDataHandle(uint8_t RXData)
{
	if((BTCommStartFlag==END)){		
		if((RXData!='\n')&&(RXData!='\r')){		
			BTCommStartFlag=START;		
			BTLength[BTRow]=0;		
			BTError[BTRow]=NO_ERROR;
			BTBuffer[BTRow][BTLength[BTRow]]=RXData;		
			BTLength[BTRow]++;		
		}
	}
	else if(RXData=='\n'){		
		BTBuffer[BTRow][BTLength[BTRow]]=RXData;		
		BTLength[BTRow]++;		
		BTBuffer[BTRow][BTLength[BTRow]]='\0';	
		IsOK=0;		
		switch(BTBuffer[BTRow][0]){		
			case 'O':
								IsOK=1;
									break;
							
			case 'E':		
								IsOK=2;
								break;
								
			case 'S':
								DEBUG("DATACHANGE\r\n");
								BTSet(BTBuffer[BTRow]+1);
								ReWriteDatas();
								InitDevicesDatas();
								break;
			//case 'o':BTOpen(BTBuffer[BTRow]+1);break;
								
		}	
		if(BTRow<(BTMAXROW-1))		
			BTRow++;		
		else
			BTRow=0;		
		BTCommStartFlag=END;		
		
	}
	else if(RXData!='\r'){		
		BTBuffer[BTRow][BTLength[BTRow]]=RXData;		
		if(BTLength[BTRow]<=(BTMAXBUFFER-3))		
			BTLength[BTRow]++;		
		else
			BTError[BTRow]=ERROR;		
	}
}

extern uint8_t SystemCanStart;
void BTSet(uint8_t *Datas)
{
	DEBUG("set\r\n");
	uint8_t i;
//	for(i=0;i<3;i++){
//		if(Datas[i]==SetAuth[i])
//			continue;
//		else
//			return ;
//	}
//	Datas+=3;
	if(SystemCanStart==0){
		switch(Datas[0]){
			DEBUG("CHANGEDATA");
			case 'i':
								Datas++;
								for(i=0;Datas[i]!='\n';i++)
									IPAddr[i]=Datas[i];
								IPAddr[i]='\0';	break;
			case 'p':
								Datas++;
								for(i=0;Datas[i]!='\n';i++)
									Port[i]=Datas[i];
								Port[i]='\0';break;
			case 'C':
								Datas++;
								for(i=0;Datas[i]!='\n';i++)
									ClientID[i]=Datas[i];
								ClientID[i]='\0';break;
//			case 'U':
//								Datas++;
//								for(i=0;Datas[i]!='\n';i++)
//									Username[i]=Datas[i];
//								Username[i]='\0';break;
//			case 'P':
//								Datas++;
//								for(i=0;Datas[i]!='\n';i++)
//									Password[i]=Datas[i];
//								Password[i]='\0';break;
//			case 'N':
//								Datas++;
//								for(i=0;Datas[i]!='\n';i++)
//									BTName[i]=Datas[i];
//								BTName[i]='\0';break;
		}
		
	}
}