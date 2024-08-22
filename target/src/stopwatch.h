#ifndef STOPWATCH_H_
#define STOPWATCH_H_

// *******************************************************
// 
// stopwatch.h
//
// Author: S Fernando
// 
// *******************************************************
#include <stdint.h>
#include <stdbool.h>
// *******************************************************
#define MAX_LAPS 10

// *******************************************************
// toggleStopwatch: Cycles between started and stopped stopwatch
void toggleStopwatch (void);
// *******************************************************
// resetStopwatch: Resets stopwatch and lap times
void resetStopwatch (void);
// *******************************************************
// readStopwatch: Reads the current time value of the stopwatch
// Should be polled frequently
uint32_t readStopwatch (void);
// *******************************************************
// storeLap: Stores the current stopwatch time value in the lap array and
// advances the lap count and lap index
void storeLap (void);
// *******************************************************
// readLap: Reads the most recent lap's time value or a previous one if 
// a negative argument is given
uint32_t readLap (int8_t lapIndexMod);
// *******************************************************
// getLapIndex: Peeks at the current lapReadIndex
int8_t getLapIndex (void);

#endif /*STOPWATCH_H_*/