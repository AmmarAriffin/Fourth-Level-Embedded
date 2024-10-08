
#include "unitConverter.h"


static UnitType unitType;

// Constants
#define M_PER_STEP 9/10 // Meters per step
#define KM_TO_MILES 62/100
#define MS_TO_KMH 36/10


uint32_t changeToMiles(uint32_t numSteps)
{
    numSteps = numSteps*M_PER_STEP*KM_TO_MILES;
    return (numSteps);
}

uint32_t changeToKM(uint32_t numSteps)
{
    numSteps = numSteps*M_PER_STEP;
    return (numSteps);
}


int32_t changeToFahrenheit(int32_t tempCelcius)
{
    return ((tempCelcius * 9/5) + 32);
}



uint32_t changeToMPH(uint32_t numSteps, uint8_t seconds)
{
    uint32_t speed;
    if (seconds != 0) {
        speed = (numSteps / seconds) * M_PER_STEP * KM_TO_MILES;
    } else {
        speed = numSteps * M_PER_STEP * KM_TO_MILES;
    }
    return (speed);
}

uint32_t changeToKPH(uint32_t numSteps, uint8_t seconds)
{
    uint32_t speed;
    if (seconds != 0) {
        speed = (numSteps / seconds) * M_PER_STEP * MS_TO_KMH;
    } else {
        speed = numSteps * MS_TO_KMH * M_PER_STEP;
    }
    return (speed);
}


void incrementUnitType(void)
{
    unitType++;
    if (unitType == NUM_UNITS) {
        unitType = UNIT_STEPS;
    }
}

UnitType getUnit(void)
{
    return unitType;
}