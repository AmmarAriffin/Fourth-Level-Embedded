// *******************************************************
// 
// timer_s.c
//
// Author: S Fernando
// 
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "timer_s.h"
#include "core.h"
#include "utils/ustdlib.h"
#include "step_counter_main.h"

#define MS_IN_SECOND 100
#define MS_IN_MINUTE 6000
#define MS_IN_HOUR 360000
#define EXT_MS 100
#define EXT_S 60
#define EXT_M 60
#define EXT_H 24 
#define NUM_TIMERS 4

/* Local declarations */
static timer_s * createTimer (uint8_t ID);

static uint8_t timerSelect = 0;
static uint8_t placeSelect = 0;

struct timer_s
{
    uint32_t lastReadTime;
    uint32_t timeRemaining;
    uint32_t timeTotal;
    uint8_t id;
    bool isRunning;
};

static timer_s *timerArray[NUM_TIMERS];



void initTimers(void)
{
    for (uint8_t i=0; i < NUM_TIMERS; i++)
    {
        timerArray[i] = createTimer(i + 1);
    }
} 


void toggleTimer(void) 
{
    uint32_t currentTime = readCurrentTick()/TICK_MOD;

    if (timerArray[timerSelect]->isRunning) {
        timerArray[timerSelect]->isRunning = false;
    } else {
        timerArray[timerSelect]->isRunning = true;
        timerArray[timerSelect]->lastReadTime = currentTime;
    }
}


void resetTimer(void) 
{
    // timerArray[timerSelect]->timeTotal = 1000; //Debug functionality to easily add time to a timer
    timerArray[timerSelect]->timeRemaining = timerArray[timerSelect]->timeTotal;
    timerArray[timerSelect]->isRunning = false;
}


void updateTimers(void) 
{
    uint32_t currentTime = readCurrentTick()/TICK_MOD;
    static char msg[17];
    uint8_t i;
    for (i=0;i < NUM_TIMERS;i++) {
        if (timerArray[i]->isRunning) {
            if ((currentTime - timerArray[i]->lastReadTime) >= timerArray[i]->timeRemaining) {
                timerArray[i]->isRunning = false;
                timerArray[i]->timeRemaining = 0;
                usnprintf(msg, 17, "TIMER %d ENDED!", timerArray[i]->id);
                flashMessage(msg);
            } else {
                timerArray[i]->timeRemaining = timerArray[i]->timeRemaining - (currentTime - timerArray[i]->lastReadTime);
                timerArray[i]->lastReadTime = currentTime;
            }
        }
    }
}


uint32_t readTimer(uint8_t timerIndex) 
{
    uint32_t currentTime = readCurrentTick()/TICK_MOD;

    static char msg[17];
    if (timerArray[timerIndex]->isRunning) {
        if ((currentTime - timerArray[timerIndex]->lastReadTime) >= timerArray[timerIndex]->timeRemaining) {
            timerArray[timerIndex]->isRunning = false;
            timerArray[timerIndex]->timeRemaining = 0;
            usnprintf(msg, 17, "TIMER %d ENDED!", timerArray[timerIndex]->id);
            flashMessage(msg);
            return timerArray[timerIndex]->timeRemaining;
        } else {
            timerArray[timerIndex]->timeRemaining = timerArray[timerIndex]->timeRemaining - (currentTime - timerArray[timerIndex]->lastReadTime);
            timerArray[timerIndex]->lastReadTime = currentTime;
            return timerArray[timerIndex]->timeRemaining;
        }
    } else {
        if (timerArray[timerIndex]->timeRemaining == timerArray[timerIndex]->timeTotal) {
            return timerArray[timerIndex]->timeTotal;
        }
    }
    return timerArray[timerIndex]->timeRemaining;
}


uint8_t getTimerID (uint8_t timerIndex) 
{
    return timerArray[timerIndex]->id;
}


void incrementTime (void)
{
    static uint32_t seconds;
    static uint32_t minutes;
    static uint32_t hours;

    timerArray[timerSelect]->timeTotal /= EXT_MS;
    seconds = timerArray[timerSelect]->timeTotal % EXT_S;
    timerArray[timerSelect]->timeTotal /= EXT_S;
    minutes = timerArray[timerSelect]->timeTotal % EXT_M;
    timerArray[timerSelect]->timeTotal /= EXT_M;
    hours = timerArray[timerSelect]->timeTotal;

    switch(placeSelect) {
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
    timerArray[timerSelect]->timeTotal = (seconds*MS_IN_SECOND) + (minutes*MS_IN_MINUTE) + (hours*MS_IN_HOUR);
    timerArray[timerSelect]->timeRemaining = timerArray[timerSelect]->timeTotal;
    timerArray[timerSelect]->isRunning = false;
}


void decrementTime (void)
{
    static uint32_t seconds;
    static uint32_t minutes;
    static uint32_t hours;

    timerArray[timerSelect]->timeTotal /= EXT_MS;
    seconds = timerArray[timerSelect]->timeTotal % EXT_S;
    timerArray[timerSelect]->timeTotal /= EXT_S;
    minutes = timerArray[timerSelect]->timeTotal % EXT_M;
    timerArray[timerSelect]->timeTotal /= EXT_M;
    hours = timerArray[timerSelect]->timeTotal;

    switch(placeSelect) {
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
    timerArray[timerSelect]->timeTotal = (seconds*MS_IN_SECOND) + (minutes*MS_IN_MINUTE) + (hours*MS_IN_HOUR);
    timerArray[timerSelect]->timeRemaining = timerArray[timerSelect]->timeTotal;
    timerArray[timerSelect]->isRunning = false;
}


void setTime (uint8_t timerIndex, uint32_t time)
{
    timerArray[timerIndex]->timeTotal = time;
}


void timerCycle(void)
{
    timerSelect = (timerSelect + 1) % NUM_TIMERS;
}


void placeCycle(void)
{
    placeSelect = (placeSelect + 1) % 3;
}


uint8_t getSelectedTimer(void)
{
    return timerSelect;
}


uint8_t getSelectedPlace(void)
{
    return placeSelect;
}

/* Local functions */

// *******************************************************
// createTimer: Creates and initialises a timer's members
// e.g timer_s *myTimer = createTimer(1);
static timer_s * createTimer (uint8_t ID)
{
    timer_s *instance = malloc(sizeof(timer_s));
    if (instance) {
        instance->lastReadTime = 0;
        instance->timeRemaining = 0;
        instance->timeTotal = 0;
        instance->id = ID;
        instance->isRunning = false;
    }
    return instance;
}