
#include "unity.h"
#include "average.h"


static averager averageBuff;


void setUp(void)
{
    initAverager(&averageBuff);
}

void tearDown(void)
{
}


void test_average_full_buffer(void) 
{
    // Arrange
    for (uint8_t i = 0; i < 10; i++) {
        storeData(&averageBuff, 6);
    }

    // Act
    uint32_t value = getAverage(&averageBuff);

    // Assert
    TEST_ASSERT_EQUAL(6, value);
}


void test_average_leftovers_when_buffer_is_not_full(void)
{
    // Arrange
    for (uint8_t i = 0; i < 5; i++) {
        storeData(&averageBuff, 6); // Fill Half of buffer only
    }

    // Act
    uint32_t value = getAverage(&averageBuff);

    // Assert
    TEST_ASSERT_EQUAL(6, value);
}