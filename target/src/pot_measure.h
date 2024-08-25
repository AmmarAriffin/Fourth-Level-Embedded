/* 
 *   by Ammar Ariffin
 *
 * Potentiometer Functionality
**/


#ifndef ADC_READ_H_
#define ADC_READ_H_

#include <stdint.h>

//*****************************************************************************
//
// The handler for the ADC conversion complete interrupt.
// Writes to the circular buffer.
//
//*****************************************************************************
void pollPot(void);

void callbackPotADC(int32_t value);

void initPotADC(void);

int32_t getPotVal(void);

#endif //ADC_READ_H_
