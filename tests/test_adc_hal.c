#include "unity.h"
#include "adc_hal.h"

// #include "ADC_read.h" // REMOVE AT SOME POINT

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circBufT_mock.h"
#include "tiva_mocks/adc_mock.h"
#include "tiva_mocks/sysctl_mock.h"
#include "inc/hw_memmap.h"

/* Helper functions */
void reset_fff(void)
{
    FFF_ADC_FAKES_LIST(RESET_FAKE);
    FFF_SYSCTL_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void dummyCallback(uint32_t value) 
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

void test_adc_hal_registers_an_adc0(void)
{ 
    // Arrange
    // Act
    adcHalRegister(0, dummyCallback);
    // Assert
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_ADC0, SysCtlPeripheralEnable_fake.arg0_val);
}

void test_adc_hal_registers_an_adc1(void)
{ 
    // Arrange
    // Act
    adcHalRegister(1, dummyCallback);
    // Assert
    TEST_ASSERT_EQUAL(SYSCTL_PERIPH_ADC1, SysCtlPeripheralEnable_fake.arg0_val);
}

void test_adc_hal_arranges_sequence_correctly(void)
{ 
    // Arrange
    // Act
    adcHalRegister(0, dummyCallback);
    // Assert
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceConfigure_fake.arg0_val);
    TEST_ASSERT_EQUAL(3, ADCSequenceConfigure_fake.arg1_val);
    TEST_ASSERT_EQUAL(ADC_TRIGGER_PROCESSOR, ADCSequenceConfigure_fake.arg2_val);
    TEST_ASSERT_EQUAL(0, ADCSequenceConfigure_fake.arg3_val);
}

void test_adc_hal_does_not_initialise_adc_with_invalid_id(void)
{ 
    // Arrange
    // Act
    adcHalRegister(42, dummyCallback);
    // Assert
    TEST_ASSERT_NOT_EQUAL(SYSCTL_PERIPH_ADC0, SysCtlPeripheralEnable_fake.arg0_val);
    TEST_ASSERT_NOT_EQUAL(SYSCTL_PERIPH_ADC1, SysCtlPeripheralEnable_fake.arg0_val);
}

void test_adc_hal_does_not_initialise_adc_with_invalid_callback(void)
{ 
    // Arrange
    // Act
    adcHalRegister(0, NULL);
    // Assert
    TEST_ASSERT_NOT_EQUAL(SYSCTL_PERIPH_ADC0, SysCtlPeripheralEnable_fake.arg0_val);
    TEST_ASSERT_NOT_EQUAL(SYSCTL_PERIPH_ADC1, SysCtlPeripheralEnable_fake.arg0_val);
}
