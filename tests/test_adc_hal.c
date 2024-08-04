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

void circBuf42Callback(uint32_t value)
{
    //
    circBuf_t testBuf;
    initCircBuf(&testBuf, 10);
    writeCircBuf(&testBuf, 42);
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Custom mocks*/

int32_t ADCSequenceDataGet_fake_value(uint32_t arg0, uint32_t arg1,
                                  uint32_t *arg2)
{
    (void)arg0;
    (void)arg1;
    *arg2 = 42;
    return 0;
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

void test_adc_hal_start_conversion_triggers_adc(void)
{ 
    // Arrange
    adcHalRegister(0, dummyCallback);
    // Act
    adcHalStartConversion(0);
    // Assert
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCProcessorTrigger_fake.arg0_val);
}

void test_adc_hal_start_conversion_with_invalid_id_does_not_triggers_adc(void)
{ 
    // Arrange
    adcHalRegister(0, dummyCallback);
    // Act
    adcHalStartConversion(42);
    // Assert
    TEST_ASSERT_NOT_EQUAL(ADC0_BASE, ADCProcessorTrigger_fake.arg0_val);
}

void test_adc_hal_ISR_reads_correct_channel(void)
{ 
    // Arrange
    adcHalRegister(0, dummyCallback);
    // Act
    adcIntCallback();
    // Assert
    TEST_ASSERT_EQUAL(ADC0_BASE, ADCSequenceDataGet_fake.arg0_val);
}

void test_adc_hal_ISR_is_cleared(void)
{ 
    // Arrange
    adcHalRegister(0, dummyCallback);
    // Act
    adcIntCallback();
    // Assert
    TEST_ASSERT_EQUAL(1, ADCIntClear_fake.call_count);
}

void test_adc_hal_ISR_is_calls_correct_callback(void)
{ 
    // Arrange
    adcHalRegister(0, circBuf42Callback);
    // Act
    adcIntCallback();
    // Assert
    TEST_ASSERT_EQUAL(42, writeCircBuf_fake.arg1_val);
}

void test_adc_hal_callback_receives_correct_value(void)
{
    // Arrange
    ADCSequenceDataGet_fake.custom_fake = ADCSequenceDataGet_fake_value;
    adcHalRegister(0, dummyCallback);
    // Act
    adcIntCallback();
    // Assert
    uint32_t *ptr42; 
    ptr42 = ADCSequenceDataGet_fake.arg2_val;
    TEST_ASSERT_EQUAL(42, ptr42);
}

