#include "smoke.h"


#define N 50
#define CO_ERROR "ERROR"

uint8_t Gas_Condition;
uint8_t co_data[N];

void smoke_init()//adc �˿ڳ�ʼ��
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
    return data; //�������N������ֵ���м�ֵ��Ϊ���������
}

//û�б�������Ļ������趨�����������ѹֵΪ�ο���ѹ����ʱ��AOUT�˵ĵ�ѹ��1V���ң�
//����������⵽��������ʱ����ѹÿ����0.1V��ʵ�ʱ��������Ũ������200ppm
//���򵥵�˵��1ppm=1mg/kg=1mg/L=1��10-6 ��������ʾ����Ũ�ȣ�������ҺŨ�ȡ���
//������������Ϳ����ڵ�Ƭ�����潫��õ�ģ������ѹֵת��ΪŨ��ֵ�����Ũ�Ⱥ͵�ѹ��ϵ�ı�ֵ�������ԣ������������ԡ�

void clear_smoke_data()
{
	int i;
	for(i=0;i<N;i++)
	{
		co_data[i]='\0';
	}
}
//		ppm�������Ũ�ȣ�Parts per million ��
//����ppm����ҺŨ�ȣ�����������������һ�ֱ�ʾ������ppm��ʾ�����֮һ��
//����������Һ����1��ˮ��Һ����1/1000���������ʣ�����Ũ�ȣ���������������Ϊ1ppm��
//�����������壺�Ի�������������������Ⱦ��Ũ�ȵı�ʾ����֮һ�� 
//�������Ũ�ȱ�ʾ����һ��������Ŀ�����������Ⱦ������������ppm 
//�����󲿷�������������õ�����Ũ�ȶ������Ũ�ȣ�ppm����
//	�����ҹ��涨���ر��ǻ������ţ���Ҫ������Ũ��������Ũ�ȵĵ�λ���磺mg/m3����ʾ��
//	���ǹ��ҵı�׼�淶Ҳ���ǲ�������Ũ�ȵ�λ���磺mg/m3;����ʾ��
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
//		����Ȼ���Ļ����£�ʵ��AOUT�˵ĵ�ѹΪ0.5V������⵽��Ȼ��ʱ����ѹÿ����0.1V,ʵ�ʱ�������Ũ������200ppm
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


/*wsx*///��ʾ����Ũ��
void SMOKE_Display(const u8* gas)
{
    
    OLED_ShowString(43,0,"    ",12);//ˢ�¶�̬��ʾ��
    OLED_ShowString(43,0,gas,12);
    OLED_ShowString(67,0,"PPM",12);
    
}
