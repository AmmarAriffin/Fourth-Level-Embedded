// *******************************************************
// 
// stopwatch.c
//
// Author: S Fernando
// 
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
#include "stopwatch.h"
#include "step_counter_main.h"
#include "utils/ustdlib.h"
#include "freeRTOS.h"
#include "task.h"

#define TICK_MOD 10 // Changes ticks to milliseconds


stopWatch_s stopWatch = {.lapNum = 0, .lapReadIndex = -1, .isRunning = false};


void toggleStopwatch (void)
{
    uint32_t currentTime = xTaskGetTickCount()/TICK_MOD;

    if (stopWatch.isRunning) {
        stopWatch.isRunning = false;
    } else {
        stopWatch.isRunning = true;
        stopWatch.lastReadTime = currentTime;
        stopWatch.lapReadIndex = stopWatch.lapNum - 1;
    }
    
}


void resetStopwatch (void) 
{
    stopWatch.elapsedTime = 0;
    stopWatch.lapNum = 0;
    stopWatch.lapReadIndex = -1;
    stopWatch.isRunning = false;
    uint8_t i;
    for (i = 0; i < MAX_LAPS;i++){
        stopWatch.lapTimes[i] = 0;
    }
}


uint32_t readStopwatch (void)
{
    uint32_t currentTime = xTaskGetTickCount()/TICK_MOD;

    if (stopWatch.isRunning) {
        stopWatch.elapsedTime = stopWatch.elapsedTime + currentTime - stopWatch.lastReadTime;
        stopWatch.lastReadTime = currentTime;
        return stopWatch.elapsedTime;
    } else {
        return stopWatch.elapsedTime;
    }
}


void storeLap (void)
{
    uint32_t currentTime = xTaskGetTickCount()/TICK_MOD;

    if (stopWatch.isRunning) {
        
        if (stopWatch.lapNum < MAX_LAPS) {
            stopWatch.lapTimes[stopWatch.lapNum] = stopWatch.elapsedTime + currentTime - stopWatch.lastReadTime;
            stopWatch.lapNum++;
            stopWatch.elapsedTime = 0;
        }else {
            flashMessage("Max Laps Reached!");
        }
        stopWatch.lapReadIndex = stopWatch.lapNum - 1;
        
        
    } else {
        if (stopWatch.lapReadIndex < MAX_LAPS - 1 && stopWatch.lapReadIndex < stopWatch.lapNum) {
            stopWatch.lapReadIndex++;
        } else {
            stopWatch.lapReadIndex = 0;
        }
        
    }
}


uint32_t readLap (int8_t lapIndexMod)
{
    return stopWatch.lapTimes[stopWatch.lapReadIndex + lapIndexMod];
}


int8_t getLapIndex (void)
{
    return stopWatch.lapReadIndex;
}