/*****************\HEADER/******************
* AUTHORS: - Patty Furr                   *
*          - Rea Sons                     *
* VERSION: 2.0                            *
* CREATED: 8/16/2024, 4:43:34 PM          *
* MODIFIED: 8/17/2024, 10:12:10 AM        *
* FILE: unitConverter.h                   *
*******************************************/





#ifndef UNITCONVERTER_H_
#define UNITCONVERTER_H_
#include <stdint.h>

typedef enum {
    UNIT_STEPS = 0,
    UNIT_METRIC,
    UNIT_IMPERIAL,
    NUM_UNITS,
} UnitType;



uint32_t changeToMiles(uint32_t numSteps);

uint32_t changeToKM(uint32_t numSteps);

int32_t changeToFahrenheit(int32_t tempCelcius);

void incrementUnitType(void);

uint32_t changeToKPH(uint32_t numSteps, uint8_t seconds);

uint32_t changeToMPH(uint32_t numSteps, uint8_t seconds);

UnitType getUnit(void);

#endif // UNITCONVERTER_H_
