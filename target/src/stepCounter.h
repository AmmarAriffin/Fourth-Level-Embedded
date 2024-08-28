/* 
 *   by Ammar Ariffin
 *
 * Module for counting step and any step counting related functions
 * 
**/


#ifndef STEPCOUNTER_H_
#define STEPCOUNTER_H_


#include <stdint.h>

void pollSteps(void);

void initStepCounter(void);

uint32_t getStepsCount(void);

void incrementStep(void);

void incrementStep45(void);

void decrementStep45(void);

void resetStepCount(void);

void setGoal(uint32_t newGoal);

uint32_t getStepsPercent(void);

uint32_t getCurrentGoal(void);

#endif