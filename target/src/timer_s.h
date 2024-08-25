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

#define NUM_TIMERS 4
static timer_s *timerArray[NUM_TIMERS];


// *******************************************************
// createTimer: Creates and initialises a timer's members
// e.g timer_s *myTimer = createTimer(1);
timer_s * createTimer (uint8_t ID);
// *******************************************************
// destroyTimer: Destroys a timer
// e.g destroyTimer(&myTimer);
void destroyTimer(timer_s **instance);
// *******************************************************
// toggleTimer: Cycles between started and stopped timer
// e.g toggleTimer(myTimer);
void toggleTimer(timer_s *timerID);
// *******************************************************
// resetTimer: Returns timer's timerRemaing to timeTotal and stops the timer
// e.g resetTimer(myTimer);
void resetTimer(timer_s *timerID);
// *******************************************************
// updateTimer: Updates the timer's timeRemaining and flashes a message if finished.
// e.g updateTimer(myTimer);
void updateTimer(timer_s *timerID);
// *******************************************************
// readTimer: Both updates the timer's timeRemaining and returns it.
// Also flashes a timer ended message.
// e.g uint32_t remainingTime = readTimer(myTimer);
uint32_t readTimer(timer_s *timerID);
// *******************************************************
// getTimerID: Returns the id of a timer
// e.g uint8_t timerID = getTimerID(myTimer);
uint8_t getTimerID (timer_s *timerID);
// *******************************************************
// incrementTime: Increases the timeTotal value based on the place argument
//place == 0 -> +1 second // e.g incrementTime(myTimer, 0);
//place == 1 -> +1 minute // e.g incrementTime(myTimer, 1);
//place == 2 -> +1 hour   // e.g incrementTime(myTimer, 2);
void incrementTime (timer_s *timerID, uint8_t place);
// *******************************************************
// decrementTime: Decreases the timeTotal value based on the place argument
//place == 0 -> -1 second // e.g decrementTime(myTimer, 0);
//place == 1 -> -1 minute // e.g decrementTime(myTimer, 1);
//place == 2 -> -1 hour   // e.g decrementTime(myTimer, 2);
void decrementTime (timer_s *timerID, uint8_t place);
// *******************************************************
// setTime: Sets a timer's timeTotal to an exact amount in milliseconds.
// For use within the system rather than in UI
// e.g setTime(myTimer, 10000);
void setTime (timer_s *timerID, uint32_t time);

#endif /*TIMER_S_H_*/