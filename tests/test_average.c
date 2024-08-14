
#include "unity.h"
#include "average.h"


static averager averageBuff;

#define BUFFER_SIZE 20

void setUp(void)
{
    initAverager(&averageBuff, BUFFER_SIZE);
}

void tearDown(void)
{
}

/* Helper Functions */
void storeSomeValuesInBuffer(int32_t num, int numTimes)
{
    for (int i = 0; i < numTimes; i++) {
        storeData(&averageBuff, num);
    }
}

void test_average_full_buffer(void) 
{
    // Arrange
    storeSomeValuesInBuffer(6, BUFFER_SIZE);

    // Act
    int32_t value = getAverage(&averageBuff);

    // Assert
    TEST_ASSERT_EQUAL(6, value);
}


void test_average_leftovers_when_buffer_is_not_full(void)
{
    // Arrange
    storeSomeValuesInBuffer(6, BUFFER_SIZE/2);
    
    // Act
    int32_t value = getAverage(&averageBuff);

    // Assert
    TEST_ASSERT_EQUAL(3, value);
}

void test_average_when_fulL_buffer_is_overwriting_old_values(void)
{
    // Arrange : do 10 full buffer
    storeSomeValuesInBuffer(6, BUFFER_SIZE);
    // Move read_index to middle
    getAverage(&averageBuff);
    // then overwrite 5 with new values
    storeSomeValuesInBuffer(2, BUFFER_SIZE/2);

    // Act
    int32_t value = getAverage(&averageBuff);
    // Assert : 2 * 5 + 6 * 5 = 40 / 10 = 4
    TEST_ASSERT_EQUAL(4, value);
}

void test_average_can_return_negative_values(void)
{
    // Arrange : buffer with negative values
    storeSomeValuesInBuffer(-6, BUFFER_SIZE);

    // Act : getAverage
    int32_t value = getAverage(&averageBuff);
    // Assert
    TEST_ASSERT_EQUAL(-6, value);
}


void test_average_still_return_running_total_after_multiple_reads(void)
{
    // Arrange : buffer with 5 values and is already read and 
    //           another 5 values that has not been read
    storeSomeValuesInBuffer(6, BUFFER_SIZE);
    getAverage(&averageBuff);
    storeSomeValuesInBuffer(6, BUFFER_SIZE/2);

    // Act
    int32_t value = getAverage(&averageBuff);

    // Assert
    TEST_ASSERT_EQUAL(6, value);
}

