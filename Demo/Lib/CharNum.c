#include "CharNum.h"

int ChangeFloatToChar(FLOAT_TYPE Number,unsigned int Points,char Save[])
{
    int Amount;
    int DecimalAmount;
    int Count_i;
    FLOAT_TYPE Decimal;
		if((Number>-1)&&(Number<0)){
			Save[0]='-';
			Amount=ChangeIntToChar((INT_TYPE)Number,Save+1);
			Amount++;
		}
		else
			Amount=ChangeIntToChar((INT_TYPE)Number,Save);
    if(Points==0)
        return Amount;
    else{
        Save[Amount]='.';
        Amount++;
        Decimal=(Number-(INT_TYPE)Number);
        if(Decimal<0)
            Decimal=-Decimal;
        for(Count_i=0;Count_i<Points;Count_i++){
            Decimal*=10;
            if((INT_TYPE)Decimal==0){
                Save[Amount]='0';
                Amount++;
            }
        }
        DecimalAmount=ChangeIntToChar((INT_TYPE)Decimal,Save+Amount);
        return Amount+DecimalAmount;
    }
}
int ChangeIntToChar(INT_TYPE Number,char Save[])
{
    INT_TYPE NumberTemp;
    char IsMinus=0;
    int Count_i=-1;
    int Amount;
    if(Number<0){
        IsMinus=1;
        Number=-Number;
        Save[0]='-';
        Count_i++;
    }
    NumberTemp=Number;
    do{
        NumberTemp/=10;
        Count_i++;
    }while(NumberTemp!=0);
    Amount=Count_i+1;
    Save[Amount]='\0';
    if(IsMinus==1){
        for(;Count_i>0;Count_i--){
           Save[Count_i]=Number%10+48;
           Number/=10;
        }
    }
    else{
        for(;Count_i>=0;Count_i--){
           Save[Count_i]=Number%10+48;
           Number/=10;
        }
    }
    return Amount;
}
FLOAT_TYPE ChangeCharToFloat(char Save[])
{
    int Count_i;
    int Point_i=0;
    int Index=0;
    INT_TYPE NumberTemp1=0,NumberTemp2=0;
    FLOAT_TYPE Number;
    if(Save[0]=='-')
        Index++;
    for(;Save[Index]!='.';Index++){
        NumberTemp1*=10;
        NumberTemp1+=Save[Index]-48;
    }
    Point_i=Index+1;
    NumberTemp2=ChangeCharToInt(Save+Point_i);
    Number=NumberTemp2;
    for(;Save[Index]!='\0';Index++);
    for(Count_i=0;Count_i<Index-Point_i;Count_i++)
        Number/=10;
    Number+=NumberTemp1;
    if(Save[0]=='-')
        Number=-Number;
    return Number;
}
INT_TYPE ChangeCharToInt(char Save[])
{
    int Count_i=0;
    INT_TYPE Number=0;
    if(Save[0]=='-')
        Count_i++;
    for(;Save[Count_i]!='\0';Count_i++){
        Number*=10;
        Number+=Save[Count_i]-48;
    }
    if(Save[0]=='-')
        Number=-Number;
    return Number;
}




