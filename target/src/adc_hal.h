#ifndef ADC_HAL_H_
#define ADC_HAL_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "ADC_read.h"

//*****************************************************************************
//
// Author:  S Fernando	
// 
//
//*****************************************************************************

#define numEntries 2

void adcHalRegister(uint32_t adcIndex, void (*callback)(uint32_t));
void adcHalIntHandler(void);
void adcHalStartConversion(uint32_t adcIndex);
// typedef void(*callback)(uint32_t);

#endif //ADC_HAL_H_