#ifndef TIMER_S_H_
#define TIMER_S_H_

// *******************************************************
// 
// timer_s.h
//
// Author: S Fernando
// 
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
// *******************************************************

typedef struct timer_s timer_s;

// *******************************************************
// createTimer: Creates and initialises a timer's members
timer_s * createTimer (uint8_t ID);
// *******************************************************
// destroyTimer: Destroys a timer
void destroyTimer (timer_s *instance);
// *******************************************************
// toggleTimer: Cycles between started and stopped timer
void toggleTimer(timer_s *timerID);
// *******************************************************
// resetTimer: Returns timer's timerRemaing to timeTotal and stops the timer
void resetTimer(timer_s *timerID);
// *******************************************************
// updateTimer: Updates the timer's timeRemaining and flashes a message if finished.
void updateTimer(timer_s *timerID);
// *******************************************************
// readTimer: Both updates the timer's timeRemaining and returns it.
// Also flashes a timer ended message.
uint32_t readTimer(timer_s *timerID);
// *******************************************************
// getTimerID: Returns the id of a timer
uint8_t getTimerID (timer_s *timerID);
// *******************************************************
// incrementTime: Increases the timeTotal value based on the place argument
//place == 0 -> +1 second
//place == 1 -> +1 minute
//place == 2 -> +1 hour
void incrementTime (timer_s *timerID, uint8_t place);
// *******************************************************
// decrementTime: Decreases the timeTotal value based on the place argument
//place == 0 -> -1 second
//place == 1 -> -1 minute
//place == 2 -> -1 hour
void decrementTime (timer_s *timerID, uint8_t place);
// *******************************************************
// setTime: Sets a timer's timeTotal to an exact amount in milliseconds.
// For use within the system rather than in UI
void setTime (timer_s *timerID, uint32_t time);

#endif /*TIMER_S_H_*/