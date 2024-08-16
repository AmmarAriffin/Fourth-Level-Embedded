
#include <stdint.h>


typedef struct {
    uint32_t stepsTaken;
    uint32_t stepsGoal;
} StepCounter;

static StepCounter stepCounter;

uint32_t getStepsCount(void)
{
    return (stepCounter.stepsTaken);
}

void incrementStep(void)
{
    stepCounter.stepsTaken++;
}

void resetStepCount(void)
{
    stepCounter.stepsTaken = 0;
}

void setGoal(uint32_t newGoal)
{
    stepCounter.stepsGoal = newGoal;
}

uint32_t getStepsPercent(void)
{
    return (stepCounter.stepsTaken / stepCounter.stepsGoal * 100);
}

uint32_t getCurrentGoal(void)
{
    return (stepCounter.stepsGoal);
}


