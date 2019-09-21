#include "BC26Task.h"

#define BC26_MQTT_INIT_1 "AT+QMTCFG=\"keepalive\",0,0\r\n"
#define BC26_MQTT_INIT_2 "AT+QMTCFG=\"session\",0,0\r\n"
#define BC26_MQTT_INIT_3 "AT+QMTCFG=\"timeout\",0,60,10,1\r\n"
#define BC26_MQTT_INIT_4 "AT+QMTCFG=\"will\",0,0,2,1,\"will\",\"disconnect\"\r\n"

Task BC26MQTTInitTask;
uint8_t BC26MQTTInitIndex=0;

uint8_t BC26MQTTInitResultHandle(void)
{
	if(BC26RX.ATCOMResult==BC26_ATCOM_OK){
		DEBUG("MQTT Init ok\r\n");
		BC26MQTTInitIndex++;
		return BC26TX_FINISH;
	}
	else
		return BC26TX_REPEAT;
}
void BC26MQTTInitCheck(Task *TaskAddr)
{
	switch(BC26MQTTInitIndex){
		case 0:
			BC26TX.TXRequest(BC26MQTTInitResultHandle,BC26_MQTT_INIT_1);
		break;
		case 1:
			BC26TX.TXRequest(BC26MQTTInitResultHandle,BC26_MQTT_INIT_2);
		break;
		case 2:
			BC26TX.TXRequest(BC26MQTTInitResultHandle,BC26_MQTT_INIT_3);
		break;
		case 3:
			BC26TX.TXRequest(BC26MQTTInitResultHandle,BC26_MQTT_INIT_4);
		break;
		case 4:
			DEBUG("ALL Init OK\r\n");
			BC26MQTTInitIndex=0;
			TaskAddr->Sleep=TASK_SLEEP;
			WakeUpNextTask(TaskAddr);
		break;
	}
}