#ifndef CORE_H_
#define CORE_H_

// *******************************************************
// 
// core.h
//
// Author: S Fernando
// 
// *******************************************************
#include <stdbool.h>

#define TICK_MOD 10 // Changes ticks to milliseconds
#define MAX_STR_LEN 16
// *******************************************************
// readCurrentTick: Read the current systick value, without mangling the data
unsigned long readCurrentTick(void);
// *******************************************************
// flashMessage: Used by other modules to write a message
void flashMessage(char* toShow);
// *******************************************************
// displayFlashMessage: Flash a message onto the screen, overriding everything else
void displayFlashMessage(void);
// *******************************************************
// isFlashing: Returns whether a message should be flashed or not
bool isFlashing(void);

#endif /*CORE_H_*/
