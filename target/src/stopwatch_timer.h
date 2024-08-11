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
typedef struct 
{
    uint16_t lastReadTime;
    uint16_t elapsedTime;
    uint16_t lapTimes[2];
    bool isRunning;
} stopWatch_t;
// *******************************************************
// countDown: Function definition
// void countDown(uint16_t timeSecs);
void toggleStopwatch (uint16_t currentTime);
void resetStopwatch (void);
uint16_t readStopwatch (uint16_t currentTime);


#endif /*STOPWATCH_TIMER_H_*/
