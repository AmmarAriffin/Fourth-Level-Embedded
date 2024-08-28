
#include <stdint.h>
#include "stepCounter.h"

#define DEFAULT_GOAL 100

typedef struct {
    uint32_t stepsTaken;
    uint32_t stepsGoal;
} StepCounter;

static StepCounter stepCounter;


void initStepCounter(void)
{
    stepCounter.stepsGoal = DEFAULT_GOAL;
}

uint32_t getStepsCount(void)
{
    return (stepCounter.stepsTaken);
}

void incrementStep(void)
{
    stepCounter.stepsTaken++;
}

void incrementStep45(void)
{
    stepCounter.stepsTaken+= 45;
}

void decrementStep45(void)
{
    if (stepCounter.stepsTaken < 45){
        stepCounter.stepsTaken = 0;
    } else {
        stepCounter.stepsTaken-= 45;
    }

    // (currentTime - lastReadTime) >= timeRemaining)
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
    return (stepCounter.stepsTaken * 100 / stepCounter.stepsGoal);
}

uint32_t getCurrentGoal(void)
{
    return (stepCounter.stepsGoal);
}


