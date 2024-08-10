#ifndef ADC_HAL_TIVA_H_
#define ADC_HAL_TIVA_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"

//*****************************************************************************
//
// Author:  S Fernando	
// 
//
//*****************************************************************************

#define NUM_ENTRIES 2
// *******************************************************
// callback: Defines a function pointer type for the 
// callback function which will be passed into adchalRegister.
typedef void(*callback)(uint32_t);

// *******************************************************
// adcHalRegister: Initialises an adc based on a given id.
// Registers an internal interupt function which calls the 
// callback function.
void adcHalRegister(uint32_t index, void (*callback)(uint32_t));

// *******************************************************
// adcIntCallback: Calls the passed callback function and
// clears the interrupt.
void adcIntSelectCallback(uint32_t index);

// *******************************************************
// adcHalStartConversion: Starts a conversion using the
// initialised adc.
void adcHalStartConversion(uint32_t index);

void adcCallback0(void);

void adcCallback1(void);

#endif //ADC_HAL_TIVA_H_