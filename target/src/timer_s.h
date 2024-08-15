#ifndef TIMER_S_H_
#define TIMER_S_H_

// *******************************************************
// 
// stopwatch_timer.h
//
// Author: S Fernando
// 
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
// *******************************************************

typedef struct 
{
    uint32_t lastReadTime;
    uint32_t timeRemaining;
    uint32_t timeTotal;
    uint8_t id;
    bool isRunning;
} timer_s;

void initTimer (timer_s *timerID, uint8_t a);
void toggleTimer(timer_s *timerID);
void resetTimer(timer_s *timerID);
uint32_t readTimer(timer_s *timerID);
uint8_t getTimerID (timer_s *timerID);
void incrementTime (timer_s *timerID, uint8_t place);
void decrementTime (timer_s *timerID, uint8_t place);

#endif /*TIMER_S_H_*/