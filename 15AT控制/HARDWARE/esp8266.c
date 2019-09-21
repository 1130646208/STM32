#include "esp8266.h"

#include "string.h"
#include "delay.h"

                    
typedef struct
{
	char *request;
	char *responed;
}RNR;  //request&responed

//��ʽ�����Ӧ��ע��ת���ַ��Ĵ���,��β�������\r\n��
RNR ESP8266Cmd[20]={{"AT+RST\r\n","ready"},
                    {"AT+CWMODE=1\r\n","OK"},
                    {"AT+CWLAP\r\n","OK"},
                    {"AT+CWJAP=\"ChinaNet-h5Cg\",\"13582940945\"\r\n","IP"},//"AT+CWJAP=\"ChinaNet-h5Cg\",\"13580945\"\r\n","IP"
                    {"AT+CIPMUX=1\r\n","OK"},
                    {"AT+CIPSERVER=1,7894\r\n","OK"},
                    {"AT+CIPSEND=0,11\r\n","OK"},
                    {"",""}
                  };

/*��ʼ��ESP8266*/
void ESP8266_Init(uint32_t br)
{
    uart_init(br);//����1��ʼ��
    ESP8266_CMD_Handle(0,8);
    UsartClearRXBuff();

    
    ESP8266_CMD_Handle(1,5);
    UsartClearRXBuff();

    
    ESP8266_CMD_Handle(2,5);
    UsartClearRXBuff();

    ESP8266_CMD_Handle(3,10);
    UsartClearRXBuff();

    ESP8266_CMD_Handle(4,5);
    UsartClearRXBuff();

    ESP8266_CMD_Handle(5,5);
    UsartClearRXBuff();

}
/*��ESP8266�������������������Ӧ�Ƿ���ȷ��1��ȷ0����2��ʱ3��������*/
//���ý��������ֶ���ջ�����������
//����˵������һ����������cmdx����Ӧ�ṹ������ESP8266Cmd[cmdx]�����ڶ����ǳ�ʱʱ�䣨�룩

uint8_t  ESP8266_CMD_Handle(uint8_t cmdx,uint8_t timeout)
{
    bool oot=false;//��ʱ��־ true��ʾ��ʱ
    bool get_err=false;//���յ�ERROR
    bool get_right=false;//���յ���ȷָ��
    uint8_t temp=0;//�ڼ���������
    
    uint8_t * esp8266Respond[USART_BUF_NUM];
    //��ָ�����鸳ֵ��ָ����������
    for (temp=0;temp<USART_BUF_NUM;temp++)
    {
        esp8266Respond[temp]=USART_RX_BUF[temp];
    }
    
    //��������                                       
    UsartSentDatas(USART1,ESP8266Cmd[cmdx].request);//��������
    delay_ms(100);//�����ջ�����һ������ʱ��
    //��ʾ��OLED��
    OLED_Clear();
    OLED_ShowString(0,0,"SEND:",16);
    OLED_ShowString(41,0,(u8*)ESP8266Cmd[cmdx].request,16);
    OLED_Refresh_Gram();
    //��ʼ��ʱ
    TIM_Cmd(TIM6,ENABLE);

    temp=0;
    //�������л����������˷�һЩʱ�䡣�����е�ģ���ֻ���ػ��У������еĻ�����Ϊ�գ����Բ����Կջ�������Ϊ������־����
    while(1) {
        if(INT_TIMES>=timeout){
            oot=true;
            break;
        }
        if(strstr((char *)esp8266Respond[temp],"ERROR")!=NULL){
            get_err=true;
            break;
        }
        if(strstr((char *)esp8266Respond[temp],ESP8266Cmd[cmdx].responed)!=NULL){
            get_right=true;
            break;
        }
        temp++;
        if(temp >= USART_BUF_NUM)
            temp=0;
    }
    
    //ֹͣ��ʱ����ʱ��������
    TIM_Cmd(TIM6,DISABLE);//���ü�ʱ��
    INT_TIMES=0;//��ռ�����
    
    //���Ӧ����ȷ����1�����յ�ERROR�򷵻�0����ʱ����2.UsartRXComplete()��λ���ձ�־
    if(get_right)
    {
        OLED_Clear();
        OLED_ShowString(0,0,"RECV:",16);
        OLED_ShowString(0,41,esp8266Respond[temp],16);
        OLED_Refresh_Gram();

        UsartRXComplete();
        return 1;
    }
    else if(get_err)
    {
        get_err=false;
        OLED_Clear();
        OLED_ShowString(0,0,"ERR :",16);
        OLED_ShowString(0,41,esp8266Respond[temp],16);
        OLED_Refresh_Gram();
        
        UsartRXComplete();
        return 0;
    }
    else if(oot)
    {
        oot=false;
        OLED_Clear();
        OLED_ShowString(0,0,"OOT :",16);
        OLED_ShowString(0,41,esp8266Respond[temp],16);
        OLED_Refresh_Gram();
        
        UsartRXComplete();
        return 2;
    }
    else
    {
        
        OLED_Clear();
        OLED_ShowString(0,0,"UNK :",16);
        OLED_ShowString(0,41,esp8266Respond[temp],16);
        OLED_Refresh_Gram();
        
        UsartRXComplete();
        return 3;
    }
}

char * ESP8266_Listen()
{
    return "OK";
    
    
}

bool ESP8266_Send_Data(char *data,uint8_t length)
{
    if (ESP8266_CMD_Handle(6,5)==1)
    {UsartSentDatas(USART1,data);
        
    }
    UsartClearRXBuff();
        UsartRXComplete();
    return true;
}
