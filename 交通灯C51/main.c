#include <reg52.h>
#define uint unsigned int 
#define uchar unsigned char

int D_S[10]={0x03,0x9F,0x25,0x0D,0x99,0x49,0x41,0x1F,0x01,0x09};//0,1,2,3,4,5,6,7,8,9
//绿灯（不闪）  
//红灯（不闪）
//黄灯（闪）
//flag=1,前后绿，flag=0,左右绿
int UD_t=6;//前后时间t
int LR_t=9;//左右时间t
sbit W_up_down=P2^0;//前后数码管位选
sbit W_left_right=P2^1;//左右数码管位选
sbit UD_G_led=P2^2;//前后绿
sbit UD_R_led=P2^3;//前后红
sbit UD_Y_led=P2^4;//前后黄
sbit LR_G_led=P2^5;//左右绿
sbit LR_R_led=P2^6;//左右红
sbit LR_Y_led=P2^7;//左右黄

bit flag=1;//(前后 1)和（左右 0）判断标志位

void delay()//延时函数1ms
{
	int i;
	for(i=0;i<1000;i++);  //
}
void up_down()//选通上下数码管
{				 
	W_up_down=1;	
	W_left_right=0;	
}

void left_right()//选通左右数码管
{
	W_up_down=0;
	W_left_right=1;
}
void Time0_Init()//定时器0初始化
{
	TMOD = 0X01;//选择为定时器0模式，工作方式1（16位不重装载定时），仅用TR0打开启动。
	TH0 = (65536-50000)/256;	//给定时器赋初值，定时50ms
	TL0 = (65536-50000)%256;	
	ET0 = 1;//打开定时器0中断允许
	EA = 1;//打开总中断
	TR0 = 1;//打开定时器
	UD_G_led=0;//上下绿灯点亮
	LR_R_led=0;//左右红灯点亮
}

void main()
{	  
	  Time0_Init();
	  while(1)	 //动态扫描显示
	  {	  
			P0=D_S[UD_t];//段码
			up_down();	 //上下通行
			delay();	 //延时一段时间，利用视觉暂留效应达到可视效果
			P0=0x00;	 //消隐
			
			P0=D_S[LR_t]; //同上
			left_right();
			delay();
			P0=0x00;	
	  }
}
void Timer0() interrupt 1	  //定时器0中断服务函数50ms进入一次
{
	static uint i;	 //引入静态变量 i，中断20次也就是1秒的时候才执行功能
	TH0 = (65536-50000)/256;	//给定时器赋初值，定时50ms
	TL0 = (65536-50000)%256;	
	i++;
	if(i==20)  //到一秒
	{
		i=0;
		UD_t--;
		LR_t--;
		if(flag==1)	// 上下通行
		{		
			LR_R_led=0;	//左右红灯亮
			UD_R_led=1;	//上下红灯灭
			if(LR_t<3)//进入黄灯
			{
				if(UD_t<0&&LR_t!=0)
				UD_t=2;
				UD_G_led=1;			//上下绿灯灭
				UD_Y_led=!UD_Y_led;	//黄灯闪烁

			}		
			else
			{
				UD_G_led=0;	 //绿灯亮
			}
			if(LR_t<0)		  //如果到了交换路口的时候
			{
				UD_t=9;		//进入下一状态		 
				LR_t=6;
				flag=0;
				UD_R_led=0;
				UD_G_led=1;
				LR_R_led=1;
				LR_G_led=0;
			}		
		}
		else  //flag=0左右走
		{
			UD_R_led=0;
			LR_R_led=1;			
			if(UD_t<3)//进入黄灯
			{
				if(LR_t<0&&UD_t!=0)
				LR_t=2;
				LR_G_led=1;
				LR_Y_led=!LR_Y_led;

			}		
			else
			{
				LR_G_led=0;	
			}
			if(UD_t<0)
			{
				UD_t=6;
				LR_t=9;
				flag=1;
				LR_R_led=0;
				LR_G_led=1;
				UD_R_led=1;
				UD_G_led=0;
			}			
		}		
	}	
}

