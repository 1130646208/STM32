#include "BC26RX.h"
#include "BC26Task_GetIP_Check.h"
#include "BC26Task_Device_Check.h"
#include "BC26TASK_MQTT_Sub.h"
#include "BC26TASK_MQTT_Unsub.h"
#include "BC26TASK_MQTT_Pub.h"
#include "DEBUG.h"
#include "Flash.h"
#include "CharNum.h"

uint8_t BC26RxCommStartFlag=BC26RX_END;

uint8_t BC26RxBuffer[BC26RX_MAXROW][BC26RX_MAXBUFFER];
uint8_t BC26RxLength[BC26RX_MAXROW];
uint8_t BC26RxError[BC26RX_MAXROW];
int8_t BC26RxRow=0;

uint8_t BC26RxSpecialRX[BC26RX_MAXROW][BC26RX_MAXBUFFER];
uint8_t BC26RxSpecialRXLength[BC26RX_MAXROW];
uint8_t BC26RxSpecialRXError[BC26RX_MAXROW];
uint8_t BC26RxSpecialRXNew[BC26RX_MAXROW];
int8_t BC26RxSpecialRXRow=0;

uint8_t BC26RxATStartRow;
uint8_t BC26RxATEndRow;
uint8_t BC26RxATStatus=BC26RX_END;

uint8_t BC26ResponseJudge(uint8_t *String1,uint8_t *String2,uint16_t Counts);
uint8_t BC26ReceivesTopic(uint8_t *Receives,uint8_t *Topic);

Task BC26ResponseCheckTask;

void MQTT_SUB_QUERY_Function(uint8_t *Datas);
void MQTT_SUB_ACTION_Function(uint8_t *Datas);

void BC26ResponseCheck(Task *TaskAddr)		
{	
	uint8_t *ResponseTemp,NewMsg=0;
	if((BC26RxSpecialRXRow==0)&&(BC26RxSpecialRXNew[BC26RX_MAXROW-1]==1)){	
		BC26RxSpecialRXNew[BC26RX_MAXROW-1]=0;		
		NewMsg=1;		
		ResponseTemp=BC26RxSpecialRX[BC26RX_MAXROW-1];		
	}
	else if((BC26RxSpecialRXNew[BC26RxSpecialRXRow-1]==1)&&(BC26RxSpecialRXRow!=0)){		
		BC26RxSpecialRXNew[BC26RxSpecialRXRow-1]=0;		
		NewMsg=1;		
		ResponseTemp=BC26RxSpecialRX[BC26RxSpecialRXRow-1];		
	}
	
	if(NewMsg==1){
		DEBUG("NEW MSG:");
		DEBUG(ResponseTemp);			
		if(BC26ResponseJudge(ResponseTemp,"+QMTRECV:",9)==1){		
			if(BC26ReceivesTopic(ResponseTemp,MQTT_SUB_ACTION)==1){
				DEBUG("TOPIC MQTT_SUB_ACTION GET.\r\n");
				MQTT_SUB_ACTION_Function(ResponseTemp);
			}
		}
	}
}
void MQTT_SUB_ACTION_Function(uint8_t *Datas)
{
	uint8_t i,j;
	for(i=0,j=0;j!=3;i++)
		if(Datas[i]=='\"')
			j++;
	Datas+=i;
	if((Datas[0]=='1')&&(Datas[1]=='0')&&(Datas[2]=='1')){
		if(PublicQueue[MQTT_PUB_ACTION_INDEX]==BC26MQTT_NO_PUB){
			PublicQueue[MQTT_PUB_ACTION_INDEX]=BC26MQTT_PUB;
			PublicQueueData[MQTT_PUB_ACTION_INDEX]=1;
		}
	}
	else{
		if(PublicQueue[MQTT_PUB_ACTION_INDEX]==BC26MQTT_NO_PUB){
			PublicQueue[MQTT_PUB_ACTION_INDEX]=BC26MQTT_PUB;
			PublicQueueData[MQTT_PUB_ACTION_INDEX]=0;
		}
	}
}
uint8_t BC26ResponseJudge(uint8_t *String1,uint8_t *String2,uint16_t Counts)		
{
	for(uint8_t i=0;i<Counts;i++){
		if(String1[i]==String2[i])
			continue;
		else 
			return 0;
	}
	return 1;
}
uint8_t BC26ReceivesTopic(uint8_t *Receives,uint8_t *Topic)		
{
	uint8_t i;
	for(i=0;Receives[i]!='\"';i++);
	i++;
	Receives+=i;
	for(i=0;Topic[i]!='\0';i++){
		if(Topic[i]==Receives[i])
			continue;
		else 
			return 0;
	}
	return 1;
}
void BC26DataHandle(uint8_t RXData)
{
	if((BC26RxCommStartFlag==BC26RX_END)){		
		if((RXData!='\n')&&(RXData!='\r')){		
			BC26RxCommStartFlag=BC26RX_START;		
			BC26RxLength[BC26RxRow]=0;		
			BC26RxError[BC26RxRow]=BC26RX_NO_ERROR;
			BC26RxBuffer[BC26RxRow][BC26RxLength[BC26RxRow]]=RXData;		
			BC26RxLength[BC26RxRow]++;		
		}
	}
	else if(RXData=='\n'){		
		BC26RxBuffer[BC26RxRow][BC26RxLength[BC26RxRow]]=RXData;		
		BC26RxLength[BC26RxRow]++;		
		BC26RxBuffer[BC26RxRow][BC26RxLength[BC26RxRow]]='\0';		
		switch(BC26RxBuffer[BC26RxRow][0]){		
			case 'A':			
								BC26RxATStartRow=BC26RxRow;break;
			case 'L':		
								BC26ConnectStatus=BC26CONNECT;break;
			case 'O':
								BC26RxATStatus=BC26RX_END;		
								BC26RxATEndRow=BC26RxRow;break;
			case 'E':		
								BC26RxATStatus=BC26RX_END;		
								BC26RxATEndRow=BC26RxRow;break;
			case '+':		
								if(BC26RxATStatus!=BC26RX_START){		
									for(int i=0;i<=BC26RxLength[BC26RxRow];i++){	
										BC26RxSpecialRX[BC26RxSpecialRXRow][i]=BC26RxBuffer[BC26RxRow][i];		
										BC26RxSpecialRXNew[BC26RxSpecialRXRow]=BC26RX_SPECIALRX_NEW;		
										BC26RxSpecialRXLength[BC26RxSpecialRXRow]=BC26RxLength[BC26RxRow];
										BC26RxSpecialRXError[BC26RxSpecialRXRow]=BC26RxError[BC26RxRow];
									}
									if(BC26RxSpecialRXRow<(BC26RX_MAXROW-1))		
										BC26RxSpecialRXRow++;		
									else 
										BC26RxSpecialRXRow=0;		
									BC26RxRow--;		
								}break;		
		}
			
		if(BC26RxRow<(BC26RX_MAXROW-1))		
			BC26RxRow++;		
		else
			BC26RxRow=0;		
		BC26RxCommStartFlag=BC26RX_END;		
		
	}
	else if(RXData!='\r'){		
		BC26RxBuffer[BC26RxRow][BC26RxLength[BC26RxRow]]=RXData;		
		if(BC26RxLength[BC26RxRow]<=(BC26RX_MAXBUFFER-3))		
			BC26RxLength[BC26RxRow]++;		
		else
			BC26RxError[BC26RxRow]=BC26RX_ERROR;		
	}
}
