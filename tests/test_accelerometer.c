#include "unity.h"
#include "accelerometer.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL

#include "circBufT.h"
#include "adc_hal_tiva_mock.h"


/* Helper Functions */
void reset_fff(void)
{
    FFF_ADC_HAL_TIVA_FAKES_LIST(RESET_FAKE)
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
}

void tearDown(void)
{

}

/* Test Cases */

void test_accel_read_at_same_time(void)
{
    // Arrange : given accelerometer buffer with data

    // Act : Read from AccelBuf

    // Assert
}


void test_accel_write_at_same_time(void)
{
    // Arrange : given accelerometer buffer

    // Act : Write to AccelBuf

    // ASSERT : all buffers in AccelBuf have values at the same time
}
