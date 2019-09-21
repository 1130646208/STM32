#include "water.h"
#include "Delay.h"
#include "stm32f10x_gpio.h"
#include "DEBUG.h"
#include "OLED.h"
#define a 0.01
#define N 6
u8 water_data[N];
u8 Water_Condition;//判断水位参数状态，状态正常正常发送信息，状态错误，瞬间发送信息。0异常 1正常
/*
    功能：初始化水位传感器引脚
    返回值：无
*/
void water_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_ADC2 | RCC_APB2Periph_AFIO, ENABLE); 
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_Init(GPIOB ,&GPIO_InitStructure); 
	//wsx：选择水位传感器的IO口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_6|GPIO_Pin_7; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC2, &ADC_InitStructure); 
	ADC_RegularChannelConfig(ADC2, ADC_Channel_9, 1, ADC_SampleTime_239Cycles5); 
	ADC_Cmd(ADC2, ENABLE);
	ADC_ResetCalibration(ADC2); 
	while(ADC_GetResetCalibrationStatus(ADC2));
	ADC_StartCalibration(ADC2); 
	while(ADC_GetCalibrationStatus(ADC2));
	ADC_SoftwareStartConvCmd(ADC2, ENABLE); 
}
/*
    功能：选通水位传感器
    返回值：无
*/

void choose_water(u8 choose)
{
    switch(choose)
    {
        case 12:
            GPIO_ResetBits(GPIOA,GPIO_Pin_6);
            GPIO_ResetBits(GPIOA,GPIO_Pin_7);
            GPIO_SetBits(GPIOA,GPIO_Pin_1);
            
            break;
        case 13:
            GPIO_ResetBits(GPIOA,GPIO_Pin_1);
            GPIO_ResetBits(GPIOA,GPIO_Pin_7);
            GPIO_SetBits(GPIOA,GPIO_Pin_6);
            
            break;
        case 14:
            GPIO_ResetBits(GPIOA,GPIO_Pin_1);
            GPIO_ResetBits(GPIOA,GPIO_Pin_6);
            GPIO_SetBits(GPIOA,GPIO_Pin_7);
            
            break;
        case 15:
            GPIO_SetBits(GPIOA,GPIO_Pin_1);
            GPIO_SetBits(GPIOA,GPIO_Pin_6);
            GPIO_SetBits(GPIOA,GPIO_Pin_7);
            break;
    }
    
}

/*
    功能：获取水位AD值
    返回值：水位AD值
*/
u8 get_water_data(void)					
{
    
    float value=0;int i;
    for(i=0;i<N;i++)
    {
        ADC_SoftwareStartConvCmd(ADC2, ENABLE);
        while(!ADC_GetFlagStatus(ADC2, ADC_FLAG_EOC));
        water_data[i]=ADC_GetConversionValue(ADC2)/4096.0*255;	//防止溢出
        value+=water_data[i];
    }
        return value/N;

}

/*
    功能：清空水位信息字符串数组
    返回值：无
*/
void clear_water_data()
{
	int i;
	for(i=0;i<N;i++)
	{
		water_data[i]='\0';
	}
}
/*
    功能：返回0,1,2,3水位，给别的函数调用
    返回值：无
*/
u8 get_water_level(void)//
{
	u8 data1=0,data2=0,data3=0;
    
    choose_water(12);
	data1=get_water_data();
    //DEBUG("传感器1");DEBUG2(data1);
    choose_water(13);
    data2=get_water_data();
    //DEBUG("传感器2");DEBUG2(data2);
    choose_water(14);
    data3=get_water_data();
    //DEBUG("传感器3");DEBUG2(data3);
	if(data3>200)
	{
		return 3;
	}
	else if(data2>200)
	{
		return 2;
	}
    else if(data1>200)
    {
        return 1;
    }
    else
        return 0;
}
/*
    功能：返回水位状态，给人看
    返回值：无
*/

u8 * get_water_state(u8 data)
{
	if(data==0)
	{
		clear_water_data();
        water_data[0]='0';
		
	}
	else if (data==1)
	{
		clear_water_data();
        water_data[0]='1';
	}
    else if (data==2)
	{
		clear_water_data();
        water_data[0]='2';
	}
    else 
    {
        clear_water_data();
        water_data[0]='3';
    }
    
	return water_data;
}
/*
    功能：判断水位是否超过预警值
    返回值：无
*/
void Decide_Water_ERROR(int data)//警报用
{
    if(data==3)
	{
			Water_Condition=0;
	}
	else
	{
			Water_Condition=1;		
	}
}
/*
    功能：显示水位信息到OLED
    返回值：无
*/
void WATER_Display(const u8* water)
{
    //OLED_Fill(43,13,72,24,0);//刷新动态显示区
    OLED_ShowString(43,13,"LEVEL",12);
    OLED_ShowString(67,13,water,12);

}
