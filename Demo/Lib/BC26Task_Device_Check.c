#include "BC26Task_Device_Check.h"
#include "BC26Task_GetIP_Check.h"
#include "BC26TX.h"
#include "BC26RX.h"
#include "BC26.h"
#include "Timer.h"
#include "DEBUG.h"


/**************************/
uint8_t BC26ConnectStatus=BC26DISCONNECT;
Task BC26ConnectCheckTask;

//void BC26ConnectCheck(Task *TaskAddr)
//{
//	static Timer CheckTimer;
//	if(BC26ConnectStatus==BC26DISCONNECT){
//		if(CheckTimer.Using==TIMER_NO_USING)
//			TimerAdd(&CheckTimer,500,1);
//		else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
//			DEBUG("BC26 NOT CONNECTED.\r\n");
//			TimerSub(&CheckTimer);
//			BC26Reset();
//		}
//	}
//	else{
//		if(CheckTimer.Using==TIMER_USING){
//			TimerSub(&CheckTimer);
//		}
//		if((BC26GetIPCheckTask.Sleep==TASK_SLEEP)&&(BC26GetIP==BC26NO_GET_IP)){
//			DEBUG("BC26 CONNECTED.\r\n");
//			BC26GetIPCheckTask.Sleep=TASK_NO_SLEEP;
//		}
//	}
//}

void BC26ConnectCheck(Task *TaskAddr)
{
    static Timer CheckTimer;
    //DEBUG("enter BC26ConnectCheck");
    if(BC26ConnectStatus==BC26DISCONNECT){
        if(CheckTimer.Using==TIMER_NO_USING)
            TimerAdd(&CheckTimer,500,1);
        else if(TimerLoopTimeOut(&CheckTimer)==TIMER_TIMEOUT){
            DEBUG("BC26 NOT CONNECTED.\r\n");
            TimerSub(&CheckTimer);
            BC26Reset();
        }
    }
    else{
        if(CheckTimer.Using==TIMER_USING){
            TimerSub(&CheckTimer); 
        }
        if((BC26GetIPCheckTask.Sleep==TASK_SLEEP)&&(BC26GetIP==BC26NO_GET_IP)){
            DEBUG("BC26 CONNECTED.\r\n");
            BC26GetIPCheckTask.Sleep=TASK_NO_SLEEP;
        }
    }
}