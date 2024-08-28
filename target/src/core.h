#ifndef CORE_H_
#define CORE_H_

// *******************************************************
// 
// core.h
//
// Author: S Fernando
// 
// *******************************************************

#define TICK_MOD 10 // Changes ticks to milliseconds
// *******************************************************
// readCurrentTick: Read the current systick value, without mangling the data
unsigned long readCurrentTick(void);
// *******************************************************
// flashMessage: Flash a message onto the screen, overriding everything else
// void flashMessage(char* toShow);
#endif /*CORE_H_*/
