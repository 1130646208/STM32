#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "delay.h"
#include "myiic.h"
#include "stm32f10x.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������V3
//SSD1306 OLED ����IC��������
//������ʽ:8080����/4�ߴ���
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/1/14
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 


//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
u8 OLED_GRAM[128][8];	 

//�����Դ浽LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Cmd (0xb0+i);    //����ҳ��ַ��0~7��
		OLED_WR_Cmd (0x00);      //������ʾλ�á��е͵�ַ
		OLED_WR_Cmd (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i]); 
	}   
}
//��SSD1306д��һ���ֽڡ�
//dat:Ҫд�������/����
//cmd:����/�����־ 0,��ʾ����;1,��ʾ����;
void OLED_WR_Byte(u8 data) 
{ 
    IIC_Start(); 
    IIC_Send_Byte(0x78);//OLED��ַ 
    IIC_Wait_Ack(); 
    IIC_Send_Byte(0x40);//�Ĵ�����ַ 
    IIC_Wait_Ack(); 
    IIC_Send_Byte(data); 
    IIC_Wait_Ack(); 
    IIC_Stop(); 
}

void OLED_WR_Cmd(u8 cmd) 
{ 
    IIC_Start(); 
    IIC_Send_Byte(0x78);//OLED��ַ 
    IIC_Wait_Ack(); 
    IIC_Send_Byte(0x00);//�Ĵ�����ַ 
    IIC_Wait_Ack(); 
    IIC_Send_Byte(cmd); 
    IIC_Wait_Ack(); 
    IIC_Stop(); 
}
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Cmd(0X8D);  //SET DCDC����
	OLED_WR_Cmd(0X14);  //DCDC ON
	OLED_WR_Cmd(0XAF);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Cmd(0X8D);  //SET DCDC����
	OLED_WR_Cmd(0X10);  //DCDC OFF
	OLED_WR_Cmd(0XAE);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//������ʾ
}
/*************************************************************/
//���� 
//x:0~127
//y:0~63
//t:1 ��� 0,���				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//������Χ��.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 �������ĶԽ�����
//ȷ��x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,���;1,���	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	//OLED_Refresh_Gram();//������ʾ
}
//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���
	chr=chr-' ';//�õ�ƫ�ƺ��ֵ		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[chr][t];	//����1608����
		else if(size==24)temp=asc2_2412[chr][t];	//����2412����
		else return;								//û�е��ֿ�
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n����
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//��ʾ�ַ���
//x,y:�������  
//size:�����С 
//*p:�ַ�����ʼ��ַ 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }
}	   
//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	
    
	delay_ms(100);
				  
	OLED_WR_Cmd(0xAE); //�ر���ʾ
	OLED_WR_Cmd(0xD5); //����ʱ�ӷ�Ƶ����,��Ƶ��
	OLED_WR_Cmd(80);   //[3:0],��Ƶ����;[7:4],��Ƶ��
	OLED_WR_Cmd(0xA8); //��������·��
	OLED_WR_Cmd(0X3F); //Ĭ��0X3F(1/64) 
	OLED_WR_Cmd(0xD3); //������ʾƫ��
	OLED_WR_Cmd(0X00); //Ĭ��Ϊ0

	OLED_WR_Cmd(0x40); //������ʾ��ʼ�� [5:0],����.
													    
	OLED_WR_Cmd(0x8D); //��ɱ�����
	OLED_WR_Cmd(0x14); //bit2������/�ر�
	OLED_WR_Cmd(0x20); //�����ڴ��ַģʽ
	OLED_WR_Cmd(0x02); //[1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10;
	OLED_WR_Cmd(0xA1); //���ض�������,bit0:0,0->0;1,0->127;
	OLED_WR_Cmd(0xC0); //����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·��
	OLED_WR_Cmd(0xDA); //����COMӲ����������
	OLED_WR_Cmd(0x12); //[5:4]����
		 
	OLED_WR_Cmd(0x81); //�Աȶ�����
	OLED_WR_Cmd(0xEF); //1~255;Ĭ��0X7F (��������,Խ��Խ��)
	OLED_WR_Cmd(0xD9); //����Ԥ�������
	OLED_WR_Cmd(0xf1); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Cmd(0xDB); //����VCOMH ��ѹ����
	OLED_WR_Cmd(0x30); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

	OLED_WR_Cmd(0xA4); //ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����)
	OLED_WR_Cmd(0xA6); //������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ	    						   
	OLED_WR_Cmd(0xAF); //������ʾ	 
	OLED_Clear();
}  





























