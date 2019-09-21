#ifndef __DATASUM_H
#define __DATASUM_H
/**
    传感器数据处理函数
*/
#include "hx711.h"
#include "infrared.h"
#include "key.h"
#include "oled.h"
#include "smoke.h"
#include "water.h"
#include "DEBUG.h"
#include "Delay.h"
#include "BC26Task.h"

#include "string.h"
#include "myitoa.h"

typedef struct
{
    u8* smoke_orign_data;//
    
    u8* water_orign_data;//
    u8* water_orign_state;
    
    u8 infrared_orign_data;//

    
    int hx711_orign_data;
    u8* hx711_orign_state;//
    
    char data_combine[30];
     
}SensorDataTypeDef;


extern SensorDataTypeDef SensorData;

void get_smoke_orign_data(void);

void get_water_orign_data(void);

void get_infrared_origin_data(void);

void get_hx711_origin_data(void);

void show_status(void);


void all_sensor_init(void);

void collect_data(void);

void setTPC(void);
#endif
