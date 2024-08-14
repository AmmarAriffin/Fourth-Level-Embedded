#ifndef TEMP_MEASURE_H_
#define TEMP_MEASURE_H_

#include <stdint.h>

void pollTemp(void);

void callbackADCTemp(int32_t value);

void initTempADC(void);

int32_t getTemp();

#endif