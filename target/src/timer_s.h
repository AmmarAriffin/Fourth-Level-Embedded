#ifndef TIMER_S_H_
#define TIMER_S_H_

// *******************************************************
// 
// timer_s.h
//
// Author: S Fernando
// 
// Notes:
// Rationale for updateTimer and readTimer both updating
// timeRemaining makes sense because the UI should indicate
// exactly what the timer value was when the display updated
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
// *******************************************************

typedef struct timer_s timer_s;

// *******************************************************
// initTimers: Creates and initialises all timers' members
void initTimers(void);
// *******************************************************
// toggleTimer: Cycles between started and stopped timer
void toggleTimer(void);
// *******************************************************
// resetTimer: Returns timer's timerRemaing to timeTotal and stops the timer
void resetTimer(void);
// *******************************************************
// updateTimer: Updates all running timers' timeRemaining and flashes a message if finished.
void updateTimers(void);
// *******************************************************
// readTimer: Both updates the timer's timeRemaining and returns it.
// Also flashes a timer ended message.
uint32_t readTimer(uint8_t timerIndex); 
// *******************************************************
// getTimerID: Returns the id of a timer
uint8_t getTimerID (uint8_t timerIndex); 
// *******************************************************
// incrementTime: Increases the timeTotal value
void incrementTime (void);
// *******************************************************
// decrementTime: Decreases the timeTotal value
void decrementTime (void);
// *******************************************************
// setTime: Sets a timer's timeTotal to an exact amount in milliseconds.
// For use within the system rather than in UI
// e.g setTime(5, 10000);
void setTime (uint8_t timerIndex, uint32_t time);
// *******************************************************
// timerChange: Cycles timers
void timerCycle(void);
// *******************************************************
// placeChange: Cycles place value
void placeCycle(void);

#endif /*TIMER_S_H_*/