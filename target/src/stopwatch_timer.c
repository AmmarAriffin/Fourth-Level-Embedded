// *******************************************************
// 
// stopwatch_timer.c
//
// Author: S Fernando
// 
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
#include "stopwatch_timer.h"
#include "step_counter_main.h"


//Pseudocode
// timer: 
// - buttons: start/pause/reset/increment timer
// - up button start/pause (hold for reset)
// - down button (hold to start setup) -> press to change timer digit place 
//                                  -> (use other button to change digit)
//                                  -> Change timer number

stopWatch_t stopWatch = {.lapNum = 0, .lapReadIndex = -1};


void toggleStopwatch (uint32_t currentTime)
{
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


uint32_t readStopwatch (uint32_t currentTime)
{
    if (stopWatch.isRunning) {
        stopWatch.elapsedTime = stopWatch.elapsedTime + currentTime - stopWatch.lastReadTime;
        stopWatch.lastReadTime = currentTime;
        return stopWatch.elapsedTime;
    } else {
        return stopWatch.elapsedTime;
    }
}


void storeLap (uint32_t currentTime)
{
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
