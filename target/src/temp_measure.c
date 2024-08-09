/*
 * Module to read temperature from the internal temperature sensor
 * Created 4/8/24
 * Author: Ammar Ariffin
 * 
 *
 * 
 */

/*
 \\ Feels very similar to ADC_read might integrate into one module 
 \\ Temperature only unsigned
*/

#include <stdint.h>
#include <stdbool.h>
#include "temp_measure.h"
#include "adc_hal_tiva.h"
#include "average.h"

#define MAX_ADC_VOLTAGE 3.3

#define MIN_ADC_VOLTAGE 0

#define ADC_TO_TEMP_CONST (75*(MAX_ADC_VOLTAGE - MIN_ADC_VOLTAGE)/4096)

#define ADC_CONST_TO_BE_MINUSED 147.5

#define ADC_ID_2 2 // Using 2nd ADC

static averager averagerTemp;


void pollTemp(void)
{
    adcHalStartConversion(ADC_ID_2);
}

void callbackADCTemp(uint32_t value)
{
    storeData(&averagerTemp, value);
}

void initTempADC(void)
{
    initAverager(&averagerTemp);

    adcHalRegister(ADC_ID_2, callbackADCTemp);
}

uint32_t getTemp(void) 
{
    uint32_t val = getAverage(&averagerTemp);
    return (ADC_CONST_TO_BE_MINUSED - (val*ADC_TO_TEMP_CONST));
}

