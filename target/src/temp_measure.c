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
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "inc/hw_memmap.h"
#include "circBufT.h"

#define ADC_BUF_SIZE_TEMP 10

#define MAX_ADC_VOLTAGE 3.3

#define MIN_ADC_VOLTAGE 0

#define ADC_TO_TEMP_CONST 147.5 - (75*(MAX_ADC_VOLTAGE - MIN_ADC_VOLTAGE)/4096)


static circBuf_t ADC_inBuffer;

void pollTemp(void)
{
    //
    // Initiate a conversion
    //
    ADCProcessorTrigger(ADC1_BASE, 3);
//    g_ulSampCnt++;
}


void ADCIntHandlerTemp(void)
{
    int32_t ulValue;

    ADCSequenceDataGet(ADC1_BASE, 3, &ulValue);

    writeCircBuf(&ADC_inBuffer, ulValue);

    ADCIntClear(ADC1_BASE, 3);
}

void initTempADC (void) 
{
    initCircBuf(&ADC_inBuffer, ADC_BUF_SIZE_TEMP);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC1);

    ADCSequenceConfigure(ADC1_BASE, 3, ADC_TRIGGER_PROCESSOR, 0);

    ADCSequenceStepConfigure(ADC1_BASE, 3, 0, ADC_CTL_TS | ADC_CTL_CH3 | ADC_CTL_IE | ADC_CTL_END);

    ADCSequenceEnable(ADC1_BASE, 3);

    ADCIntRegister(ADC1_BASE, 3, ADCIntHandlerTemp);

    ADCIntEnable(ADC1_BASE, 3);
}

uint32_t readTemp() {
      uint32_t sum = 0;
      uint32_t entry = 0;
      uint16_t i = 0;

      for (i = 0; i < ADC_BUF_SIZE_TEMP; i++) {
        if (readCircBuf (&ADC_inBuffer, &entry)) { //Viewing fake arg1_val will result in seeing the address of entry
            sum = sum + entry;
        }
      }

      return (sum/ADC_BUF_SIZE_TEMP) * ADC_TO_TEMP_CONST;
}





