#include "hx711.h"

#define N 10
u8 hx711_Condition;
u8 hx711_data[N];
u8 hx711_init_val=0;

void hx711_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);
 
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_5;	//a4 a5端口初始化
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
    
	
}
//获取C传感器数据 时钟端口A5 数据端口A4
u8 get_hx711_data()//根据时序图写代码
{
	unsigned long receive = 0; 
	u8 i = 0; 
    //初始化即数据接收时
    I2C_SDA_H_C;    //SDA高电平
    I2C_SCL_L_C;    //SCL低电平
    while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4));//判断是否进行数据的读取SDA
    DelayUs(1);//延时
    for(i=0;i<24;i++)//在24个周期内开始读取数据
    {
        I2C_SCL_H_C;    //SCL高电平 A5
        receive<<=1;
        DelayUs(1);
        I2C_SCL_L_C;
        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_4))//SDA
        receive++;
        DelayUs(1);	
    }
    I2C_SCL_H_C;
    DelayUs(1);
    receive = receive^0x800000;
    I2C_SCL_L_C;
    DelayUs(1);
    
    return receive/1677721.0*255;
}

u8 Decide_Stress_ERROR(void)
{
    return hx711_Condition;//异常返回0
}
u8* get_hx711_status(u8 data)
{

	if(data>100)
	{
		clear_hx711_data();
		hx711_data[0]='E';
		hx711_data[1]='R';
		hx711_data[2]='R';
		hx711_data[3]='O';
		hx711_data[4]='R';
        hx711_Condition=0;
	
	}else{
        clear_hx711_data();
        if(data>=10&&data<100){
            hx711_data[0]=data/10+0x30;
            hx711_data[1]=((int)data)%10+0x30;
        }
        if(data>=0&&data<10){
            hx711_data[0]=((int)data)/1+0x30;
            
        }
        hx711_Condition=1;
	}
	return hx711_data;
}
void hx711_calib()
{
    u8 i;
    int temp=0;
    u8 count=5;
    //uart_init(9600);
    for (i=0;i<count;i++){
    
        temp+=get_hx711_data();
        //printf("%d\n",temp);
    }
    hx711_init_val=temp/count;
    //printf("%d\n",hx711_init_val);
}
void clear_hx711_data()
{
	int i;
	for(i=0;i<N;i++)
	{
		hx711_data[i]='\0';
	}
}

void STRESS_Display(const u8* stress)
{
    
    OLED_Fill(43,37,67,64,0);//刷新动态显示区
    OLED_ShowString(43,37,stress,12);
    OLED_ShowString(67,37,"KG",12);
    
}
