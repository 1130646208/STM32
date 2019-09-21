#include "dataSum.h"
SensorDataTypeDef SensorData;
/*
    功能：获取烟雾值
    返回值：无
*/
void get_smoke_orign_data(void)
{
    
    SensorData.smoke_orign_data=my_itoa((int)(get_smoke_data()));
    //DEBUG("\r\n");DEBUG("**smoke data:");DEBUG((char*)SensorData.smoke_orign_data);DEBUG("\r\n");
    
}
/*
    功能：获取水位值
    返回值：无
*/
void get_water_orign_data(void)
{
    
    SensorData.water_orign_data=get_water_state(get_water_level());
    //DEBUG("\r\n");DEBUG("**water data:");DEBUG((char*)SensorData.water_orign_data);DEBUG("\r\n");
    
}
/*
    功能：获取红外值
    返回值：无
*/
void get_infrared_origin_data(void)
{
    
    SensorData.infrared_orign_data=infrared_detect();
    //DEBUG("\r\n");DEBUG("**infrared data:");DEBUG2(SensorData.infrared_orign_data);DEBUG("\r\n");
    //DEBUG("\r\n");DEBUG("**smoke data:");DEBUG((char*)SensorData.smoke_orign_data);DEBUG("\r\n");
}
/*
    功能：获取压力值
    返回值：无
*/
void get_hx711_origin_data(void)
{
    //DEBUG("\r\n");DEBUG("**hx711 data:");DEBUG((char*)SensorData.hx711_orign_state);DEBUG("\r\n");
    
    u8 * temp=0;
    temp=my_utoa(get_hx711_data()-hx711_init_val);
    if (*(temp+2)=='2') 
    {
        SensorData.hx711_orign_state=my_utoa(0);
        
    }
    else  {
        SensorData.hx711_orign_state=my_utoa(get_hx711_data()-hx711_init_val);

    }
}
/*
    功能：显示传感器到OLED
    返回值：无
*/
void show_status()
{
    OLED_ShowString(0,0, "SMOKE :",12);////size:选择字体 12/16/24
    OLED_ShowString(0,13,"WATER :",12);
    OLED_ShowString(0,25,"COVER :",12);
    OLED_ShowString(0,37,"STRESS:",12);
    SMOKE_Display(SensorData.smoke_orign_data);
    WATER_Display(SensorData.water_orign_data);
    infrared_dislay(SensorData.infrared_orign_data);
    STRESS_Display(SensorData.hx711_orign_state);
    OLED_Refresh_Gram();
}

/*
    功能：初始化所有传感器
    返回值：无
*/
void all_sensor_init()
{
    
    DEBUG0("Initing sensors...");
    Usart1Init();
    smoke_init();
    water_init();
    infrared_init();
    hx711_init();
    hx711_calib();
    OLED_Init();
    DEBUG0("All sensor init ok.");
    
}
/*
    功能：整合传感器信息到一个字符串
    返回值：无
*/
void collect_data(void)//合并所有信息为一个字符串
{
    char* temp;
    u8 i;
    if(SensorData.infrared_orign_data==1)
        temp="1";
    else temp="0";
    for(i=0;i<30;i++)
        SensorData.data_combine[i]='\0';
    strcat(SensorData.data_combine,ID);
    strcat(SensorData.data_combine,"|");
    strcat(SensorData.data_combine,(char*)SensorData.hx711_orign_state);
    strcat(SensorData.data_combine,"|");
    strcat(SensorData.data_combine,(char*)SensorData.water_orign_data);
    strcat(SensorData.data_combine,"|");
    strcat(SensorData.data_combine,(char*)SensorData.smoke_orign_data);
    strcat(SensorData.data_combine,"|");
    strcat(SensorData.data_combine,temp);
    //DEBUG("\r\n");DEBUG("传感器数据：");DEBUG("\r\n");    
    //DEBUG(SensorData.data_combine);
    for (i=0;i<30;i++)
        MQTTPublish[0].DataArray[i]=SensorData.data_combine[i];
    MQTTPublish[0].DataString=MQTTPublish[0].DataArray;
    
}
/*
    功能：根据接收到的蓝牙串口信息生成Topic
    返回值：无
*/
void setTPC()
{
    char temp[20]="TOPIC:";
    strcat(TOPIC,ID);
    strcat(temp,TOPIC);
    DEBUG0(temp);
}





