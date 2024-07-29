#ifndef ADC_HAL_H_
#define ADC_HAL_H_

#include <stdint.h>

//*****************************************************************************
//
// Description
// 
//
//*****************************************************************************

#define ADC_ID_1 0x40038000  // ADC0

void adc_hal_register(uint32_t ADC_ID, uint32_t callback);
//typedef void(*callback)(uint32_t);

#endif //ADC_HAL_H_