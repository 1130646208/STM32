#include "flash.h"
#include "dma.h"
#include "Usart2.h"
#include "stm32f10x.h"
#include "delay.h"

int main()
{

    
    
    //printf("running...");
    
    
    //
    uart_init(115200);
    delay_init();
    Usart2Init();
    while(1){
        if(SetNetInfo())
            break;
    }
    ReWriteDatas();
    InitDevicesDatas();
    while(1);
}
