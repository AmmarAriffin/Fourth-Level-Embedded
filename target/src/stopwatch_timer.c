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



//Pseudocode
// stopwatch: maybe switch so 4 buttons can be used
// - up button start/stop (hold for reset?)
// - down button lap
// - start: starts incrementing time/resumes if stopped
// - stop: stops incrementing time
// - reset: sets time back to 0
// - lap: displays the time when button pressed and reset the time to 0
//        while continuing the time increment

// timer: 
// - buttons: start/pause/reset/increment timer
// - up button start/pause (hold for reset)
// - down button (hold to start setup) -> press to change timer digit place 
//                                  -> (use other button to change digit)
//                                  -> Change timer number

stopWatch_t stopWatch;


void toggleStopwatch (uint16_t currentTime)
{
    if (stopWatch.isRunning) {
        stopWatch.isRunning = false;
    } else {
        stopWatch.isRunning = true;
        stopWatch.lastReadTime = currentTime;
    }
    
}

void resetStopwatch (void) 
{
    stopWatch.elapsedTime = 0;
}

uint16_t readStopwatch (uint16_t currentTime)
{
    // currenttick - start tick = elapsedticks
    // if stopped elapsed ticks unchanged
    //if start after stop: elapsedticks incremented from 
    if (stopWatch.isRunning) {
        stopWatch.elapsedTime = stopWatch.elapsedTime + currentTime - stopWatch.lastReadTime;
        stopWatch.lastReadTime = currentTime;
        return stopWatch.elapsedTime;
    } else {
        return stopWatch.elapsedTime;
    }
    
    
}

// void countDown(uint32_t timeSecs)
// {
//     //Start countdown
// }