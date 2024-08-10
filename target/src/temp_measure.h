#ifndef TEMP_MEASURE_H_
#define TEMP_MEASURE_H_

#include <stdint.h>

void pollTemp(void);

void callbackADCTemp(uint32_t value);

void initTempADC(void);

uint32_t getTemp();

#endif