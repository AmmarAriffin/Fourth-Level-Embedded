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
typedef void(*callback)(uint32_t);
void adcHalRegister(uint32_t index, void (*callback)(uint32_t));
void adcHalStartConversion(uint32_t index);
void adcIntCallback(void);

#endif //ADC_HAL_H_