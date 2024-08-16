#ifndef STEPCOUNTER_H_
#define STEPCOUNTER_H_


#include <stdint.h>


uint32_t getStepsCount(void);

void incrementStep(void);

void resetStepCount(void);

void setGoal(uint32_t newGoal);

uint32_t getStepsPercent(void);

uint32_t getCurrentGoal(void);

#endif