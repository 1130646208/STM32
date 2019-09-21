#include "smoke.h"


#define N 50
#define CO_ERROR "ERROR"

uint8_t Gas_Condition;
uint8_t co_data[N];

void smoke_init()//adc 端口初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_ADC1 | RCC_APB2Periph_AFIO, ENABLE); 
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; //PB0
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; 
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; 
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure); 
 
	ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_239Cycles5); 
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1); 
	while(ADC_GetResetCalibrationStatus(ADC1));
	
	ADC_StartCalibration(ADC1); 
	while(ADC_GetCalibrationStatus(ADC1));
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
	
}

float get_smoke_data(void){
    char  i; 
    float data=0; 
   	float ad=0;
    for(i=0;i<N;i++)
	{
        ADC_SoftwareStartConvCmd(ADC1, ENABLE);
        while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
        ad=ad+ADC_GetConversionValue(ADC1);		
	}
	
	data=ad/N;
    return data; //将排序后N个采样值的中间值作为最后结果返回
}

//没有被测气体的环境，设定传感器输出电压值为参考电压，这时，AOUT端的电压在1V左右，
//当传感器检测到被测气体时，电压每升高0.1V，实际被测气体的浓度增加200ppm
//（简单的说：1ppm=1mg/kg=1mg/L=1×10-6 常用来表示气体浓度，或者溶液浓度。）
//根据这个参数就可以在单片机里面将测得的模拟量电压值转换为浓度值。输出浓度和电压关系的比值并非线性，而是趋于线性。

void clear_smoke_data()
{
	int i;
	for(i=0;i<N;i++)
	{
		co_data[i]='\0';
	}
}
//		ppm是体积比浓度：Parts per million 。
//　　ppm是溶液浓度（溶质质量分数）的一种表示方法，ppm表示百万分之一。
//　　对于溶液：即1升水溶液中有1/1000毫升的溶质，则其浓度（溶质质量分数）为1ppm。
//　　对于气体：对环境大气（空气）中污染物浓度的表示方法之一。 
//　　体积浓度表示法：一百万体积的空气中所含污染物的体积数，即ppm 
//　　大部分气体检测仪器测得的气体浓度都是体积浓度（ppm）。
//	而按我国规定，特别是环保部门，则要求气体浓度以质量浓度的单位（如：mg/m3）表示，
//	我们国家的标准规范也都是采用质量浓度单位（如：mg/m3;）表示。
u8 * get_smoke_level(float data)
{
	if(data>1000)
	{
		clear_smoke_data();
		co_data[0]='E';
		co_data[1]='R';
		co_data[2]='R';
		co_data[3]='O';
		co_data[4]='R';
        
	
	}
	else
	{
		
		if(data<300)
		{
			clear_smoke_data();
			co_data[0]='0';
		}
//		无天然气的环境下，实测AOUT端的电压为0.5V，当检测到天然气时，电压每升高0.1V,实际被测气体浓度增加200ppm
		else if(data>300 && data<1000){
			clear_smoke_data();
			data = (data - 300) / 100 * 200;
			if(data>1000){
				co_data[0]=data/1000+0x30;
				co_data[1]=((int)data)%1000/100+0X30;
				co_data[2]=((int)data)%100/10+0x30;
				co_data[3]=((int)data)%10+0x30;
			}
			if(data>100&&data<1000){
				co_data[0]=data/100+0x30;
				co_data[1]=((int)data)%100/10+0X30;
				co_data[2]=((int)data)%10+0x30;
			}
			if(data>10&&data<100){
				co_data[0]=data/10+0x30;
				co_data[1]=((int)data)%10+0x30;
			}
			if(data>1&&data<10){
				co_data[0]=((int)data)/1+0x30;
				
			}
			Gas_Condition=1;
		}
	}
	return co_data;
}
void Decide_Smoke_ERROR(float data)
{
	if(data>1000)
	{
			Gas_Condition=0;
	}
	else
	{
			Gas_Condition=1;		
	}

}


/*wsx*///显示烟雾浓度
void SMOKE_Display(const u8* gas)
{
    
    OLED_ShowString(43,0,"    ",12);//刷新动态显示区
    OLED_ShowString(43,0,gas,12);
    OLED_ShowString(67,0,"PPM",12);
    
}
