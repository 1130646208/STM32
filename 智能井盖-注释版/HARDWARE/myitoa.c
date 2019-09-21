#include "myitoa.h"


//    ���ܣ����¼����������Խ���ͬ���͵�����ת��Ϊ�ַ���

/*
    ���ܣ�����ݴ��ַ�������
    ����ֵ����
*/
void clear_s(unsigned char s[])
{
    int i=0;
    for (i=0;i<100;i++)
    {
        s[i]='\0';
    }    
}
/*
    ���ܣ���ת�ַ���
    ����ֵ����
*/
unsigned char *reverse(unsigned char *s)
{
    char temp;
    unsigned char *p = s;    //pָ��s��ͷ��
    unsigned char *q = s;    //qָ��s��β��
    while(*q)
        ++q;
    q--;
    
    //�����ƶ�ָ�룬ֱ��p��q����
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}
/*
 * ���ܣ�����ת��Ϊ�ַ���
 * char s[] �������Ǵ洢������ÿһλ
 */
unsigned char *my_itoa(int n)
{
    int i = 0,isNegative = 0;
    static unsigned char s[100];      //����Ϊstatic������������ȫ�ֱ���
    unsigned char *p = s;    //pָ��s��ͷ��
    unsigned char *q = s;    //qָ��s��β��
    char temp;
    if((isNegative = n) < 0) //����Ǹ�������תΪ����
    {
        n = -n;
    }
    do      //�Ӹ�λ��ʼ��Ϊ�ַ���ֱ�����λ�����Ӧ�÷�ת
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //����Ǹ��������ϸ���
    {
        s[i++] = '-';
    }
    s[i] = '\0';    //�������ַ���������
    
    while(*q)
        ++q;
    q--;
    
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    } 
    return s;
}
/*
    ���ܣ�ucharת��char*
    ����ֵ����
*/
unsigned char *my_utoa(unsigned char n)
{
    int i = 0;
    static unsigned char s[100];//����Ϊstatic������������ȫ�ֱ���
    unsigned char *p = s;    //pָ��s��ͷ��
    unsigned char *q = s;    //qָ��s��β��
    char temp;
    do//�Ӹ�λ��ʼ��Ϊ�ַ���ֱ�����λ�����Ӧ�÷�ת
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    s[i] = '\0';    //�������ַ���������
    while(*q)
        ++q;
    q--;
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    } 
    return s;
}

/*
    ���ܣ�����ת�ַ���
    ����ֵ����
*/
void Int2Str(unsigned char* str, int intnum)
{
    int i, Div = 1000000000, j = 0, Status = 0;
   //32λ�޷����������10λ����,����Div=10 0000 0000
    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + '0';//ȡ���λ ת�����ַ�
        intnum = intnum % Div;//ȥ�����λ
        Div /= 10;//��ʣ��10-iλҪת��
        if ((str[j-1] == '0') & (Status == 0))//�������λ��'0'
        {
            j = 0;
        }
        else
        {
            Status++;
        }
    }
}
