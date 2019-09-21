#include "myitoa.h"


//    功能：以下几个函数可以将不同类型的数字转化为字符串

/*
    功能：清空暂存字符串数组
    返回值：无
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
    功能：反转字符串
    返回值：无
*/
unsigned char *reverse(unsigned char *s)
{
    char temp;
    unsigned char *p = s;    //p指向s的头部
    unsigned char *q = s;    //q指向s的尾部
    while(*q)
        ++q;
    q--;
    
    //交换移动指针，直到p和q交叉
    while(q > p)
    {
        temp = *p;
        *p++ = *q;
        *q-- = temp;
    }
    return s;
}
/*
 * 功能：整数转换为字符串
 * char s[] 的作用是存储整数的每一位
 */
unsigned char *my_itoa(int n)
{
    int i = 0,isNegative = 0;
    static unsigned char s[100];      //必须为static变量，或者是全局变量
    unsigned char *p = s;    //p指向s的头部
    unsigned char *q = s;    //q指向s的尾部
    char temp;
    if((isNegative = n) < 0) //如果是负数，先转为正数
    {
        n = -n;
    }
    do      //从各位开始变为字符，直到最高位，最后应该反转
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    if(isNegative < 0)   //如果是负数，补上负号
    {
        s[i++] = '-';
    }
    s[i] = '\0';    //最后加上字符串结束符
    
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
    功能：uchar转化char*
    返回值：无
*/
unsigned char *my_utoa(unsigned char n)
{
    int i = 0;
    static unsigned char s[100];//必须为static变量，或者是全局变量
    unsigned char *p = s;    //p指向s的头部
    unsigned char *q = s;    //q指向s的尾部
    char temp;
    do//从各位开始变为字符，直到最高位，最后应该反转
    {
        s[i++] = n%10 + '0';
        n = n/10;
    }while(n > 0);
    
    s[i] = '\0';    //最后加上字符串结束符
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
    功能：整型转字符串
    返回值：无
*/
void Int2Str(unsigned char* str, int intnum)
{
    int i, Div = 1000000000, j = 0, Status = 0;
   //32位无符号数最大是10位整数,所以Div=10 0000 0000
    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + '0';//取最高位 转化成字符
        intnum = intnum % Div;//去掉最高位
        Div /= 10;//还剩下10-i位要转换
        if ((str[j-1] == '0') & (Status == 0))//忽略最高位的'0'
        {
            j = 0;
        }
        else
        {
            Status++;
        }
    }
}
