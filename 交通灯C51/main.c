#include <reg52.h>
#define uint unsigned int 
#define uchar unsigned char

int D_S[10]={0x03,0x9F,0x25,0x0D,0x99,0x49,0x41,0x1F,0x01,0x09};//0,1,2,3,4,5,6,7,8,9
//�̵ƣ�������  
//��ƣ�������
//�Ƶƣ�����
//flag=1,ǰ���̣�flag=0,������
int UD_t=6;//ǰ��ʱ��t
int LR_t=9;//����ʱ��t
sbit W_up_down=P2^0;//ǰ�������λѡ
sbit W_left_right=P2^1;//���������λѡ
sbit UD_G_led=P2^2;//ǰ����
sbit UD_R_led=P2^3;//ǰ���
sbit UD_Y_led=P2^4;//ǰ���
sbit LR_G_led=P2^5;//������
sbit LR_R_led=P2^6;//���Һ�
sbit LR_Y_led=P2^7;//���һ�

bit flag=1;//(ǰ�� 1)�ͣ����� 0���жϱ�־λ

void delay()//��ʱ����1ms
{
	int i;
	for(i=0;i<1000;i++);  //
}
void up_down()//ѡͨ���������
{				 
	W_up_down=1;	
	W_left_right=0;	
}

void left_right()//ѡͨ���������
{
	W_up_down=0;
	W_left_right=1;
}
void Time0_Init()//��ʱ��0��ʼ��
{
	TMOD = 0X01;//ѡ��Ϊ��ʱ��0ģʽ��������ʽ1��16λ����װ�ض�ʱ��������TR0��������
	TH0 = (65536-50000)/256;	//����ʱ������ֵ����ʱ50ms
	TL0 = (65536-50000)%256;	
	ET0 = 1;//�򿪶�ʱ��0�ж�����
	EA = 1;//�����ж�
	TR0 = 1;//�򿪶�ʱ��
	UD_G_led=0;//�����̵Ƶ���
	LR_R_led=0;//���Һ�Ƶ���
}

void main()
{	  
	  Time0_Init();
	  while(1)	 //��̬ɨ����ʾ
	  {	  
			P0=D_S[UD_t];//����
			up_down();	 //����ͨ��
			delay();	 //��ʱһ��ʱ�䣬�����Ӿ�����ЧӦ�ﵽ����Ч��
			P0=0x00;	 //����
			
			P0=D_S[LR_t]; //ͬ��
			left_right();
			delay();
			P0=0x00;	
	  }
}
void Timer0() interrupt 1	  //��ʱ��0�жϷ�����50ms����һ��
{
	static uint i;	 //���뾲̬���� i���ж�20��Ҳ����1���ʱ���ִ�й���
	TH0 = (65536-50000)/256;	//����ʱ������ֵ����ʱ50ms
	TL0 = (65536-50000)%256;	
	i++;
	if(i==20)  //��һ��
	{
		i=0;
		UD_t--;
		LR_t--;
		if(flag==1)	// ����ͨ��
		{		
			LR_R_led=0;	//���Һ����
			UD_R_led=1;	//���º����
			if(LR_t<3)//����Ƶ�
			{
				if(UD_t<0&&LR_t!=0)
				UD_t=2;
				UD_G_led=1;			//�����̵���
				UD_Y_led=!UD_Y_led;	//�Ƶ���˸

			}		
			else
			{
				UD_G_led=0;	 //�̵���
			}
			if(LR_t<0)		  //������˽���·�ڵ�ʱ��
			{
				UD_t=9;		//������һ״̬		 
				LR_t=6;
				flag=0;
				UD_R_led=0;
				UD_G_led=1;
				LR_R_led=1;
				LR_G_led=0;
			}		
		}
		else  //flag=0������
		{
			UD_R_led=0;
			LR_R_led=1;			
			if(UD_t<3)//����Ƶ�
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

