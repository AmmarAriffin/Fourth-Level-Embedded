#include "unity.h"
#include "ADC_read.h"
// #include "adc_hal_tiva.h"
#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "circBufT_mock.h"
#include "adc_hal_tiva_mock.h"
//REMOVE TIVA DEPENDECY
// #include "tiva_mocks/adc_mock.h"
// #include "tiva_mocks/sysctl_mock.h"

#define ADC_BUF_SIZE 10

/* Helper functions */      
void reset_fff(void)
{
    FFF_CIRCBUFT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

circBuf_t* get_circBuf_ptr_and_reset_fff(void)
{
    initADC();
    circBuf_t* buffer_ptr = initCircBuf_fake.arg0_val;
    reset_fff();
    return buffer_ptr;
}

void assert_f1_called_before_f2(void* f1, void* f2)
{
    int8_t i_f1 = -1;
    int8_t i_f2 = -1;

    for (uint8_t i = 0; i < FFF_CALL_HISTORY_LEN; i++)
    {
        if(fff.call_history[i] == NULL)
            break;

        if (i_f1 == -1 && fff.call_history[i] == f1) i_f1 = i;
        if (i_f2 == -1 && fff.call_history[i] == f2) i_f2 = i;        
    }
    
    TEST_ASSERT(i_f1 < i_f2);
}

/* Custom fakes */
// Not used but left for custom fake example 
// int32_t ADCSequenceDataGet_fake_adc_value(uint32_t arg0, uint32_t arg1,
//                                   uint32_t *arg2)
// {
//     (void)arg0;
//     (void)arg1;
//     *arg2 = FAKE_ADC_VALUE;
//     return 0;
// }


uint8_t readCircBuf_fake_sequence(circBuf_t * arg0, uint32_t * arg1)
{
    static uint32_t count = 1;
    (void)arg0;
    if (count % 2 == 0) 
    {
        *arg1 = 10;
        count++;
        return 1;
    } else {
        *arg1 = 0;
        count++;
        return 0;
    }
    
    
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{
    
}

/* Test cases - initADC */
void test_adc_init_initialises_buffer(void)
{   
    // Act
    initADC();

    // Assert
    TEST_ASSERT_EQUAL(1, initCircBuf_fake.call_count);
    TEST_ASSERT_EQUAL(ADC_BUF_SIZE, initCircBuf_fake.arg1_val);
}

/* Test cases - adcCallback */
void test_ISR_write_to_correct_buffer(void)
{
    // Act
    circBuf_t* buffer_ptr = get_circBuf_ptr_and_reset_fff();
    adcCallback(42);
    // Assert
    TEST_ASSERT_EQUAL(writeCircBuf_fake.arg0_val, buffer_ptr);
}

void test_ISR_writes_correct_value_to_buffer(void)
{
    // Arrange
    uint32_t value;
    circBuf_t* buffer_ptr = get_circBuf_ptr_and_reset_fff();
    // Act
    adcCallback(42);
    // Assert
    TEST_ASSERT_EQUAL(42, writeCircBuf_fake.arg1_val);
}

/* Test cases - readADC */
void test_readADC_reads_from_correct_buffer(void)
{
    // Arrange
    circBuf_t* buffer_ptr = get_circBuf_ptr_and_reset_fff();
    
    // Act
    initADC();
    readADC();
    // Assert
    TEST_ASSERT_EQUAL(initCircBuf_fake.arg0_val, readCircBuf_fake.arg0_val);
}

void test_readADC_averages_data_correctly(void)
{
    // Arrange
    readCircBuf_fake.custom_fake = readCircBuf_fake_sequence;
    // Act
    uint32_t value = readADC();
    // Assert
    TEST_ASSERT_EQUAL(10, value); 
}
