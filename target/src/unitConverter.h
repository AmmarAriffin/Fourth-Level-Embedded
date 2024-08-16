// *******************************************************
// 
// state.h
//
// Implementing a State Pattern 
// Ammar Ariffin
// 
// *******************************************************
#ifndef UNITCONVERTER_H_
#define UNITCONVERTER_H_
#include <stdint.h>

typedef enum {
    UNIT_STEPS = 0,
    UNIT_METRIC,
    UNIT_IMPERIAL,
} UnitType;

static UnitType unitType;

uint32_t changeToMiles(uint32_t numSteps);

uint32_t changeToKM(uint32_t numSteps);

int32_t changeToFahrenheit(int32_t tempCelcius);

void incrementUnitType(void);

uint32_t changeToKPH(uint32_t numSteps, uint8_t seconds);

uint32_t changeToMPH(uint32_t numSteps, uint8_t seconds);

#endif // UNITCONVERTER_H_
