// *******************************************************
// 
// timer_s.c
//
// Author: S Fernando
// 
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
#include "timer_s.h"
#include "step_counter_main.h"
#include "utils/ustdlib.h"

#define MS_IN_SECOND 100
#define MS_IN_MINUTE 6000
#define MS_IN_HOUR 360000
#define EXT_MS 100
#define EXT_S 60
#define EXT_M 60
#define EXT_H 24 
#define TICK_MOD 10 // Changes ticks to milliseconds

struct timer_s
{
    uint32_t lastReadTime;
    uint32_t timeRemaining;
    uint32_t timeTotal;
    uint8_t id;
    bool isRunning;
};


timer_s * createTimer (uint8_t ID)
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


void destroyTimer(timer_s *instance)
{
    if (instance) {
        free(instance);
    }
}


void toggleTimer(timer_s *timerID) 
{
    uint32_t currentTime = readCurrentTick()/TICK_MOD;

    if (timerID->isRunning) {
        timerID->isRunning = false;
    } else {
        timerID->isRunning = true;
        timerID->lastReadTime = currentTime;
    }
}


void resetTimer(timer_s *timerID) 
{
    // timerID->timeTotal = 1000; //Debug functionality to easily add time to a timer
    timerID->timeRemaining = timerID->timeTotal;
    timerID->isRunning = false;
}

void updateTimer(timer_s *timerID) 
{
    uint32_t currentTime = readCurrentTick()/TICK_MOD;
    static char msg[17];
    if (timerID->isRunning) {
        if ((currentTime - timerID->lastReadTime) >= timerID->timeRemaining) {
            timerID->isRunning = false;
            timerID->timeRemaining = 0;
            usnprintf(msg, 17, "TIMER %d ENDED!", timerID->id);
            flashMessage(msg);
        } else {
            timerID->timeRemaining = timerID->timeRemaining - (currentTime - timerID->lastReadTime);
            timerID->lastReadTime = currentTime;
        }
    }
}

uint32_t readTimer(timer_s *timerID) 
{
    uint32_t currentTime = readCurrentTick()/TICK_MOD;

    static char msg[17];
    if (timerID->isRunning) {
        if ((currentTime - timerID->lastReadTime) >= timerID->timeRemaining) {
            timerID->isRunning = false;
            timerID->timeRemaining = 0;
            usnprintf(msg, 17, "TIMER %d ENDED!", timerID->id);
            flashMessage(msg);
            return timerID->timeRemaining;
        } else {
            timerID->timeRemaining = timerID->timeRemaining - (currentTime - timerID->lastReadTime);
            timerID->lastReadTime = currentTime;
            return timerID->timeRemaining;
        }
    } else {
        if (timerID->timeRemaining == timerID->timeTotal) {
            return timerID->timeTotal;
        } else {
            return timerID->timeRemaining;
        }
        
    }
}


uint8_t getTimerID (timer_s *timerID) 
{
    return timerID->id;
}


void incrementTime (timer_s *timerID, uint8_t place)
{
    static uint32_t seconds;
    static uint32_t minutes;
    static uint32_t hours;

    timerID->timeTotal /= EXT_MS;
    seconds = timerID->timeTotal % EXT_S;
    timerID->timeTotal /= EXT_S;
    minutes = timerID->timeTotal % EXT_M;
    timerID->timeTotal /= EXT_M;
    hours = timerID->timeTotal;

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
    timerID->timeTotal = (seconds*MS_IN_SECOND) + (minutes*MS_IN_MINUTE) + (hours*MS_IN_HOUR);
    timerID->timeRemaining = timerID->timeTotal;
    timerID->isRunning = false;
}

void decrementTime (timer_s *timerID, uint8_t place)
{
    static uint32_t seconds;
    static uint32_t minutes;
    static uint32_t hours;

    timerID->timeTotal /= EXT_MS;
    seconds = timerID->timeTotal % EXT_S;
    timerID->timeTotal /= EXT_S;
    minutes = timerID->timeTotal % EXT_M;
    timerID->timeTotal /= EXT_M;
    hours = timerID->timeTotal;

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
    timerID->timeTotal = (seconds*MS_IN_SECOND) + (minutes*MS_IN_MINUTE) + (hours*MS_IN_HOUR);
    timerID->timeRemaining = timerID->timeTotal;
    timerID->isRunning = false;
}

void setTime (timer_s *timerID, uint32_t time) 
{
    timerID->timeTotal = time;
}