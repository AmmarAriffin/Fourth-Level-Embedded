#include "unity.h"
#include "adc_hal.h"

#include "ADC_read.h" // REMOVE AT SOME POINT

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circBufT_mock.h" // REMOVE AT SOME POINT
#include "tiva_mocks/adc_mock.h"
#include "tiva_mocks/sysctl_mock.h"

/* Helper functions */
void reset_fff(void)
{
    FFF_ADC_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void dummy_callback(uint32_t value) 
{

}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Test cases */

void test_adc_hal_registers_an_adc(void)
{ 
    // Arrange
    // Act
    adc_hal_register(ADC_ID_1, dummy_callback);
    // Assert
    
}
