#ifndef STOPWATCH_TIMER_H_
#define STOPWATCH_TIMER_H_

// *******************************************************
// 
// stopwatch_timer.h
//
// Author: S Fernando
// 
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
#include "step_counter_main.h"
// *******************************************************
#define MAX_LAPS 10
typedef struct 
{
    uint32_t lastReadTime;
    uint32_t elapsedTime;
    uint32_t lapTimes[MAX_LAPS];
    uint8_t lapNum;
    int8_t lapReadIndex;
    bool isRunning;
} stopWatch_t;

// *******************************************************
// toggleStopwatch: Cycles between started and stopped stopwatch
void toggleStopwatch (uint32_t currentTime);
// *******************************************************
// resetStopwatch: Resets stopwatch and lap times
void resetStopwatch (void);
// *******************************************************
// readStopwatch: Reads the current time value of the stopwatch
// Should be polled frequently
uint32_t readStopwatch (uint32_t currentTime);
// *******************************************************
// storeLap: Stores the current stopwatch time value in the lap array and
// advances the lap count and lap index
void storeLap (uint32_t currentTime);
// *******************************************************
// readLap: Reads the most recent lap's time value or a previous one if 
// a negative argument is given
uint32_t readLap (int8_t lapIndexMod);
// *******************************************************
// getLapIndex: Peeks at the current lapReadIndex
int8_t getLapIndex (void);


#endif /*STOPWATCH_TIMER_H_*/
