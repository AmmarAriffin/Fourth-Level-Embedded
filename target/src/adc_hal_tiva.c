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
// #include "circBufT.h"

//*****************************************************************************
// Constants
//*****************************************************************************
#define ADC_SEQUENCE_NUM 3
//*****************************************************************************
// Global variables
//*****************************************************************************
volatile uint32_t adcIndex; 
volatile callback registeredCallback;

typedef struct {
    uint32_t adcID;
    uint32_t adcPeripheral;
} adcID_t;

adcID_t adcIDArray[NUM_ENTRIES] = 
{
    {ADC0_BASE, SYSCTL_PERIPH_ADC0},
    {ADC1_BASE, SYSCTL_PERIPH_ADC1}
};


void adcHalRegister(uint32_t index, void (*callback)(uint32_t))
{
    //Checks for invalid arguments
    if (index < 0 || index >= NUM_ENTRIES || callback == NULL) 
    {
        return;
    }

    //Makes index global within the module
    adcIndex = index;
    //Registers the passed callback function globally to be used among the module
    registeredCallback = callback;
    
    // The ADC peripheral must be enabled for configuration and use.
    SysCtlPeripheralEnable(adcIDArray[adcIndex].adcPeripheral);
    
    // Enable sample sequence 3 with a processor signal trigger.  Sequence 3
    // will do a single sample when the processor sends a signal to start the
    // conversion.
    ADCSequenceConfigure(adcIDArray[adcIndex].adcID, ADC_SEQUENCE_NUM, ADC_TRIGGER_PROCESSOR, 0);
  
    // Configure step 0 on sequence 3.  Sample channel 0 (ADC_CTL_CH0) in
    // single-ended mode (default) and configure the interrupt flag
    // (ADC_CTL_IE) to be set when the sample is done.  Tell the ADC logic
    // that this is the last conversion on sequence 3 (ADC_CTL_END).  Sequence
    // 3 has only one programmable step.  Sequence 1 and 2 have 4 steps, and
    // sequence 0 has 8 programmable steps.  Since we are only doing a single
    // conversion using sequence 3 we will only configure step 0.  For more
    // on the ADC sequences and steps, refer to the LM3S1968 datasheet.
    ADCSequenceStepConfigure(adcIDArray[adcIndex].adcID, ADC_SEQUENCE_NUM, 0, ADC_CTL_CH0 | ADC_CTL_IE | ADC_CTL_END);    
                             
    // Since sample sequence 3 is now configured, it must be enabled.
    ADCSequenceEnable(adcIDArray[adcIndex].adcID, ADC_SEQUENCE_NUM);
  
    // Register the interrupt handler
    ADCIntRegister (adcIDArray[adcIndex].adcID, ADC_SEQUENCE_NUM, adcIntCallback);
  
    // Enable interrupts for ADC sequence 3 (clears any outstanding interrupts)
    ADCIntEnable(adcIDArray[adcIndex].adcID, ADC_SEQUENCE_NUM);

    
}


void adcIntCallback(void)
{
    //Function is registered to the interrupt register and calls the callback function passed in when adcHalRegister is called.
    
    uint32_t value;

    // Get the single sample from ADC.
    ADCSequenceDataGet(adcIDArray[adcIndex].adcID, ADC_SEQUENCE_NUM, &value);//This value should be passed to the callback
    
    //Call the callback with the read value
    registeredCallback(value);

    // Clean up, clearing the interrupt
	ADCIntClear(ADC0_BASE, 3);    
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

