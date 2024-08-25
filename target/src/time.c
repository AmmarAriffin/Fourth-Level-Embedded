// *******************************************************
// 
// time.c
//
// Author: S Fernando
// 
// *******************************************************

#include "FreeRTOS.h"
#include "task.h"


unsigned long readCurrentTick(void)
{
    return xTaskGetTickCount();
}