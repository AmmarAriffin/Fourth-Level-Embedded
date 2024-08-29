
#include <stdint.h>
#include <stdbool.h>
#include "stepCounter.h"
#include "accelerometer.h"
#include "core.h"
#include "math.h"


#define DEFAULT_GOAL 1000
#define STEP_THRESHOLD_HIGH 270
#define STEP_THRESHOLD_LOW 235

typedef struct {
    uint32_t stepsTaken;
    uint32_t stepsGoal;
} StepCounter;

static StepCounter stepCounter;

static vector3_t mean;
static bool stepHigh = false;


void pollSteps(void)
{
    pollAccelData();
    mean = getAverageAccel();
    uint16_t combined = sqrt(mean.x*mean.x + mean.y*mean.y + mean.z*mean.z);
    if (combined >= STEP_THRESHOLD_HIGH && stepHigh == false) {
        stepHigh = true;
        incrementStep();
        if (getStepsCount() == getCurrentGoal()) {
            flashMessage("Goal reached!");
        }
    } else if (combined <=  STEP_THRESHOLD_LOW) {
        stepHigh = false;
    }

}


void initStepCounter(void)
{
    stepCounter.stepsGoal = DEFAULT_GOAL;
    initAccelBuffer();
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


