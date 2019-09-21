#include "test1.h"
#include "Delay.h"
#include "DEBUG.h"
#include "OLED.h"

Task test;

void test_func(Task *TaskAddr)
{
    
    //DEBUG("\nenter test_func\n");
    get_smoke_orign_data();
    get_water_orign_data();
    get_infrared_origin_data();
    get_hx711_origin_data();
    show_status();
    collect_data(); 
    DelayMs(500);
}