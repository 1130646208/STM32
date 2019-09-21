#include "esp8266.h"

#include "string.h"
#include "delay.h"

                    
typedef struct
{
	char *request;
	char *responed;
}RNR;  //request&responed

//格式：命令，应答（注意转义字符的处理,结尾必须带上\r\n）
RNR ESP8266Cmd[20]={{"AT+RST\r\n","ready"},
                    {"AT+CWMODE=1\r\n","OK"},
                    {"AT+CWLAP\r\n","OK"},
                    {"AT+CWJAP=\"ChinaNet-h5Cg\",\"13582940945\"\r\n","IP"},//"AT+CWJAP=\"ChinaNet-h5Cg\",\"13580945\"\r\n","IP"
                    {"AT+CIPMUX=1\r\n","OK"},
                    {"AT+CIPSERVER=1,7894\r\n","OK"},
                    {"AT+CIPSEND=0,11\r\n","OK"},
                    {"",""}
                  };

/*初始化ESP8266*/
void ESP8266_Init(uint32_t br)
{
    uart_init(br);//串口1初始化
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
/*向ESP8266发送命令，并返回命令响应是否正确：1正确0错误2超时3其他错误*/
//调用结束后请手动清空缓冲区，并将
//参数说明：第一个是命令编号cmdx（对应结构体数组ESP8266Cmd[cmdx]），第二个是超时时间（秒）

uint8_t  ESP8266_CMD_Handle(uint8_t cmdx,uint8_t timeout)
{
    bool oot=false;//超时标志 true表示超时
    bool get_err=false;//接收到ERROR
    bool get_right=false;//接收到正确指令
    uint8_t temp=0;//第几个缓冲区
    
    uint8_t * esp8266Respond[USART_BUF_NUM];
    //将指针数组赋值，指向多个缓冲区
    for (temp=0;temp<USART_BUF_NUM;temp++)
    {
        esp8266Respond[temp]=USART_RX_BUF[temp];
    }
    
    //发送命令                                       
    UsartSentDatas(USART1,ESP8266Cmd[cmdx].request);//发送命令
    delay_ms(100);//给接收缓冲区一个缓冲时间
    //显示在OLED上
    OLED_Clear();
    OLED_ShowString(0,0,"SEND:",16);
    OLED_ShowString(41,0,(u8*)ESP8266Cmd[cmdx].request,16);
    OLED_Refresh_Gram();
    //开始计时
    TIM_Cmd(TIM6,ENABLE);

    temp=0;
    //遍历所有缓冲区（会浪费一些时间。但因有的模块会只返回换行，导致有的缓冲区为空，所以不能以空缓冲区作为结束标志。）
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
    
    //停止计时（计时有少许误差）
    TIM_Cmd(TIM6,DISABLE);//禁用计时器
    INT_TIMES=0;//清空计数秒
    
    //如果应答正确返回1，接收到ERROR则返回0，超时返回2.UsartRXComplete()复位接收标志
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
