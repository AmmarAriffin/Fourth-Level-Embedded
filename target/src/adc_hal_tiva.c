//*****************************************************************************
//
// adc_hal_tiva.c - Removes direct dependency of ADC_read.c on TIVA libraries
// Should be the only module calling functions from the TIVA libraries
// Author:  S Fernando	
//
//*****************************************************************************

#include <stdint.h>
#include <stddef.h>
#include "adc_hal_tiva.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define ADC_SEQUENCE_NUM 3

typedef struct {
    const uint32_t adcID;
    const uint32_t adcPeripheral;
    const uint32_t sampleChannel;
    const void (*Intcallback)(void);
    void (*GetValueCallback)(int32_t);
} adcID_t;

adcID_t adcIDArray[NUM_ENTRIES] = 
{
    {ADC0_BASE, SYSCTL_PERIPH_ADC0, ADC_CTL_CH0, adcCallback0, NULL},
    {ADC1_BASE, SYSCTL_PERIPH_ADC1, ADC_CTL_CH3 | ADC_CTL_TS, adcCallback1, NULL}
};


void adcHalRegister(uint32_t index, void (*callback)(int32_t))
{
    //Checks for invalid arguments
    if (index < 0 || index >= NUM_ENTRIES || callback == NULL) 
    {
        return;
    }

    adcIDArray[index].GetValueCallback = callback;

    // The ADC peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(adcIDArray[index].adcPeripheral);
    
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(adcIDArray[index].adcID, ADC_SEQUENCE_NUM, ADC_TRIGGER_PROCESSOR, 0);
  
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(adcIDArray[index].adcID, ADC_SEQUENCE_NUM, 0, adcIDArray[index].sampleChannel | ADC_CTL_IE | ADC_CTL_END);    
                             
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(adcIDArray[index].adcID, ADC_SEQUENCE_NUM);
  
    // Register the interrupt handler
    ADCIntRegister (adcIDArray[index].adcID, ADC_SEQUENCE_NUM, adcIDArray[index].Intcallback);
  
    // Enable interrupts for ADC sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(adcIDArray[index].adcID, ADC_SEQUENCE_NUM);

    
}


void adcIntSelectCallback(uint32_t index)
{
    uint32_t value;

    // Get the single sample from ADC.
    ADCSequenceDataGet(adcIDArray[index].adcID, ADC_SEQUENCE_NUM, &value);//This value should be passed to the callback
    
    //Call the callback with the read value
    adcIDArray[index].GetValueCallback(value);

    // Clean up, clearing the interrupt
	ADCIntClear(adcIDArray[index].adcID, 3);    
}

void adcCallback0(void)
{
    adcIntSelectCallback(0);
}

void adcCallback1(void)
{
    adcIntSelectCallback(1);
}

void adcHalStartConversion(uint32_t index)
{
    // Checks for invalid index
    if (index < 0 || index >= NUM_ENTRIES) 
    {
        return;
    }

    // Initiate a conversion
    ADCProcessorTrigger(adcIDArray[index].adcID, ADC_SEQUENCE_NUM);
}

