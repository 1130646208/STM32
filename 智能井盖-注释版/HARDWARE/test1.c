#include "test1.h"
#include "Delay.h"
#include "DEBUG.h"
#include "OLED.h"

/*
    功能：新建任务，调用传感器采集信息并显示到OLED
    返回值：无
*/
void test_func(Task *TaskAddr)
{
    get_hx711_origin_data();
    get_smoke_orign_data();
    get_water_orign_data();
    get_infrared_origin_data();
    show_status();
    collect_data();
    DelayMs(500);
}
