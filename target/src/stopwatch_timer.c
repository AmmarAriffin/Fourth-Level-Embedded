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
#include "utils/ustdlib.h"

#define MS_IN_SECOND 100
#define MS_IN_MINUTE 6000
#define MS_IN_HOUR 360000
#define EXT_MS 100
#define EXT_S 60
#define EXT_M 60
#define EXT_H 24 


stopWatch_s stopWatch = {.lapNum = 0, .lapReadIndex = -1, .isRunning = false};

timer_s timers[NUM_TIMERS] = 
{
    {.id = 1},
    {.id = 2},
    {.id = 3},
    {.id = 4}
};

extern volatile uint8_t timerSelect = 0;
extern volatile uint8_t placeSelect = 0;




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



//Pseudocode
// timer: 
// - buttons: start/pause/reset/increment timer
// - up button start/pause (hold for reset)
// - down button (hold to start setup) -> hold again to set timer
//                                     -> Change timer number
//           press up to change timer digit place                        
//           use left right to change value

// timer_t timerOne; // Maybe make an array of timers.

void toggleTimer(uint8_t timerID, uint32_t currentTime) 
{
    if (timers[timerID].isRunning) {
        timers[timerID].isRunning = false;
    } else {
        timers[timerID].isRunning = true;
        timers[timerID].lastReadTime = currentTime;
        // timerOne.lapReadIndex = timerOne.lapNum - 1;
    }
}


void resetTimer(uint8_t timerID) 
{
    timers[timerID].timeTotal = 1000;
    timers[timerID].timeRemaining = timers[timerID].timeTotal;
    timers[timerID].isRunning = false;
}


uint32_t readTimer(uint8_t timerID, uint32_t currentTime) 
{
    static char msg[17];
    if (timers[timerID].isRunning) {
        if ((currentTime - timers[timerID].lastReadTime) >= timers[timerID].timeRemaining) {
            timers[timerID].isRunning = false;
            timers[timerID].timeRemaining = 0;
            usnprintf(msg, 17, "TIMER %d ENDED!", timers[timerID].id);
            flashMessage(msg); //THIS CURRENTLY ONLY WORKS IF ON THE TIMER DISPLAY SCREEN
            return timers[timerID].timeRemaining;
        } else {
            timers[timerID].timeRemaining = timers[timerID].timeRemaining - (currentTime - timers[timerID].lastReadTime);
            timers[timerID].lastReadTime = currentTime;
            return timers[timerID].timeRemaining;
        }
    } else {
        return timers[timerID].timeRemaining;
    }
}


uint8_t getTimerID (uint8_t timerID) 
{
    return timers[timerID].id;
}


void incrementTime (uint8_t timerID, uint8_t place)
{
    static uint32_t seconds;
    static uint32_t minutes;
    static uint32_t hours;

    timers[timerID].timeTotal /= EXT_MS;
    seconds = timers[timerID].timeTotal % EXT_S;
    timers[timerID].timeTotal /= EXT_S;
    minutes = timers[timerID].timeTotal % EXT_M;
    timers[timerID].timeTotal /= EXT_M;
    hours = timers[timerID].timeTotal;

    switch(place) {
        case 0: 
            seconds = (seconds + 1) % EXT_S;
            break;
        case 1:
            minutes = (minutes + 1) % EXT_M;
            break;
        case 2:
            hours = (hours + 1) % EXT_H;
            break;
    }
    timers[timerID].timeTotal = (seconds*MS_IN_SECOND) + (minutes*MS_IN_MINUTE) + (hours*MS_IN_HOUR);
    timers[timerID].timeRemaining = timers[timerID].timeTotal;
    timers[timerID].isRunning = false;
}

void decrementTime (uint8_t timerID, uint8_t place)
{
    static uint32_t seconds;
    static uint32_t minutes;
    static uint32_t hours;

    timers[timerID].timeTotal /= EXT_MS;
    seconds = timers[timerID].timeTotal % EXT_S;
    timers[timerID].timeTotal /= EXT_S;
    minutes = timers[timerID].timeTotal % EXT_M;
    timers[timerID].timeTotal /= EXT_M;
    hours = timers[timerID].timeTotal;

    switch(place) {
        case 0: 
            seconds = (EXT_S + seconds - 1) % EXT_S;
            break;
        case 1:
            minutes = (EXT_M + minutes - 1) % EXT_M;
            break;
        case 2:
            hours = (EXT_H + hours - 1) % EXT_H;
            break;
    }
    timers[timerID].timeTotal = (seconds*MS_IN_SECOND) + (minutes*MS_IN_MINUTE) + (hours*MS_IN_HOUR);
    timers[timerID].timeRemaining = timers[timerID].timeTotal;
    timers[timerID].isRunning = false;
}