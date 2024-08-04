#ifndef TEMP_MEASURE_H_
#define TEMP_MEASURE_H_

void pollTemp(void);

void ADCIntHandlerTemp(void);

void initTempADC(void);

uint32_t readTemp();


#endif