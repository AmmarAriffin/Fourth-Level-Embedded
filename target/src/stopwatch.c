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

#define TICK_MOD 10 // Changes ticks to milliseconds

typedef struct
{
    uint32_t lastReadTime;
    uint32_t elapsedTime;
    uint32_t lapTimes[MAX_LAPS];
    uint8_t lapNum;
    int8_t lapReadIndex;
    bool isRunning;
} stopWatch_s;

stopWatch_s stopWatch = {.lapNum = 0, .lapReadIndex = -1, .isRunning = false};


void toggleStopwatch (void)
{
    uint32_t currentTime = readCurrentTick()/TICK_MOD;

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
    uint32_t currentTime = readCurrentTick()/TICK_MOD;

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
    uint32_t currentTime = readCurrentTick()/TICK_MOD;

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
    int8_t finalMod;
    // Wraps index modifier to current number of laps stored
    if (lapIndexMod > 0) {
        finalMod = (stopWatch.lapReadIndex + lapIndexMod) % stopWatch.lapNum;
    } else if (lapIndexMod < 0) {
        finalMod = (stopWatch.lapReadIndex + lapIndexMod);
        if (finalMod < 0) {
            finalMod = -finalMod % stopWatch.lapNum;
        } 
    } else {
        finalMod = stopWatch.lapReadIndex;
    }
    return stopWatch.lapTimes[finalMod];
}   


int8_t getLapIndex (void)
{
    return stopWatch.lapReadIndex;
}