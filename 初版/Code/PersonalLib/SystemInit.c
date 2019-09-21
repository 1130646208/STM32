#include "SystemInit.h"


CircularQueue *SystemStartTask=CIRCULARQUEUE_NULL;
char StringTemp[128];

void SystemTaskInit(void)
{
	TaskAdd(&BC26TX.AtCommandTask,&SystemStartTask,BC26TX.TXTask,TASK_NO_SLEEP);
    
	TaskAdd(&BC26RX.BC26RXPBufferReaderTask,&SystemStartTask,BC26RX.BC26RXPBufferReader,TASK_NO_SLEEP);
	TaskAdd(&BC26ResetTask,&SystemStartTask,BC26ResetMission,TASK_SLEEP);    //DEBUG("\n BC26ResetMission \n");
    
	TaskAdd(&BC26CheckTask,&SystemStartTask,BC26Check,TASK_NO_SLEEP);//BC26 exist    //DEBUG("\n BC26Check \n");
//	TaskAdd(&BC26PSMSetTask,&SystemStartTask,BC26PSMSet,TASK_SLEEP);//PSM Mode Set
	TaskAdd(&BC26CardCheckTask,&SystemStartTask,BC26CardCheck,TASK_SLEEP);//Card Ready
	TaskAdd(&BC26IPCheckTask,&SystemStartTask,BC26IPCheck,TASK_SLEEP);
	TaskAdd(&BC26MQTTInitTask,&SystemStartTask,BC26MQTTInitCheck,TASK_SLEEP);
	TaskAdd(&BC26OpenTask,&SystemStartTask,BC26Open,TASK_SLEEP);//do no open,Success Open
	TaskAdd(&BC26ConnectTask,&SystemStartTask,BC26Connect,TASK_SLEEP);//Do no Connect,
//	TaskAdd(&BC26UnsubTask,&SystemStartTask,BC26Unsub,TASK_SLEEP);
//	TaskAdd(&BC26SubTask,&SystemStartTask,BC26Sub,TASK_SLEEP);
	TaskAdd(&BC26PubTask,&SystemStartTask,BC26Pub,TASK_SLEEP);
    TaskAdd(&test,&SystemStartTask,test_func,TASK_NO_SLEEP);
    DelayMs(500);

}
void SystemHardwareInit(void)
{
	SystemInit();
    DelayInit();
	DelayMs(10);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	TimeInit();
	Usart1Init();
	//RTCInit(); 
	//FirstRunning();
	InitDevicesDatas();
    //BC26≥ı ºªØ
	BC26RX.HardWareInit();
	BC26GpioInit();
    SystemTaskInit();
    change_pub_status();
}

/*≤‚ ‘”√*/
void sen_test()
{

    get_hx711_origin_data();
    get_water_orign_data();
    get_smoke_orign_data();
    get_infrared_origin_data();
    collect_data();
    show_status();
    
}