//*****************************************************************************
//
// ADCdemo1.c - Simple interrupt driven program which samples with AIN0
//
// Author:  P.J. Bones	UCECE
// Last modified:	8.2.2018
//
//*****************************************************************************
// Based on the 'convert' series from 2016
//*****************************************************************************

#include <stdint.h>
#include <stdbool.h>


// #include "inc/hw_types.h"
// #include "driverlib/pwm.h"
// #include "driverlib/gpio.h"
// #include "driverlib/systick.h"
// #include "driverlib/debug.h"
// #include "utils/ustdlib.h"
// #include "driverlib/interrupt.h"

//******************************
//Enclosed should be removed upon completion of HAL
// #include "inc/hw_memmap.h"
// #include "driverlib/sysctl.h"
// #include "driverlib/adc.h"
//******************************
#include "circBufT.h"
#include "ADC_read.h"
#include "adc_hal_tiva.h"


//*****************************************************************************
// Constants
//*****************************************************************************
#define ADC_BUF_SIZE 10
#define ADC_ID_1 0
//*****************************************************************************
// Global variables
//*****************************************************************************
static circBuf_t ADC_inBuffer;		// Buffer of size BUF_SIZE integers (sample values)

//*****************************************************************************
//
// The interrupt handler for the for SysTick interrupt.
//
//*****************************************************************************
void pollADC(void)
{

    adcHalStartConversion(ADC_ID_1);

}

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void adcCallback(uint32_t value)
{
    // Place it in the circular buffer (advancing write index)
	writeCircBuf (&ADC_inBuffer, value);
	//
}


//*****************************************************************************
// Initialisation functions for the ADC
//*****************************************************************************

void initADC (void)
{
    
    initCircBuf(&ADC_inBuffer, ADC_BUF_SIZE);

    adcHalRegister(ADC_ID_1, adcCallback);

}

uint32_t readADC(void) {
      uint32_t sum = 0;
      uint32_t entry = 0;
      uint16_t i = 0;
      uint8_t readValues = 0;

      for (i = 0; i < ADC_BUF_SIZE; i++) {
        if (readCircBuf (&ADC_inBuffer, &entry)) { //Viewing fake arg1_val will result in seeing the address of entry
            sum = sum + entry;
            readValues++;
        }
      }
      //Prevents division by zero
      if (readValues == 0) {
        return sum;
      } else {
        return sum/readValues;
      }
      
}



