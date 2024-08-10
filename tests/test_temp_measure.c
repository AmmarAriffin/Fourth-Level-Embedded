
#include "unity.h"
#include "temp_measure.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "average_mock.h"
#include "adc_hal_tiva_mock.h"

void setUp(void)
{
    FFF_AVERAGE_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void tearDown(void)
{
}

void test_getTemp_get_average_temp_properly(void)
{
    // Arrange
    getAverage_fake.return_val = 2000;
    
    // Act
    uint32_t val = getTemp();

    // Assert
    TEST_ASSERT_EQUAL(26, val);
}
