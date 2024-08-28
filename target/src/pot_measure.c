/* 
 *   by Ammar Ariffin
 *
 * Potentiometer Functionality
**/


#include <stdint.h>
#include <stdbool.h>

#include "pot_measure.h"
#include "adc_hal_tiva.h"
#include "average.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define ADC_BUF_SIZE 15
#define ADC_ID_1 0
//*****************************************************************************
// Global variables
//*****************************************************************************
static averager ADC_inBuffer;		// Buffer of size BUF_SIZE integers (sample values)

void pollPot(void)
{
    adcHalStartConversion(ADC_ID_1);
}

void callbackPotADC(int32_t value)
{
  storeData(&ADC_inBuffer, value);
}

static void storeInitValues(void)
{
  for (int i=0; i < ADC_BUF_SIZE; i++) {
    pollPot();
  }
}

void initPotADC (void)
{
    initAverager(&ADC_inBuffer, ADC_BUF_SIZE);
    adcHalRegister(ADC_ID_1, callbackPotADC);
    storeInitValues(); // To fill up with proper values in buffer instead of junk
}


int32_t getPotVal(void)
{
  int32_t val = getAverage(&ADC_inBuffer);
  return (val);
}


