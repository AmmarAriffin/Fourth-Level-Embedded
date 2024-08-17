#include "unity.h"
#include "accelerometer.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL

#include "average.h"

#include "accl_hal_tiva_mock.h"

#define BUFFER_SIZE 20

/* Helper Functions */
void reset_fff(void)
{
    FFF_ACC_HAL_TIVA_FAKES_LIST(RESET_FAKE)
    FFF_RESET_HISTORY();
}

/* Unity setup and teardown */
void setUp(void)
{
    reset_fff();
    initAccelBuffer();
}

void tearDown(void)

{

}


/* Helper Functions */
void writeStuffToVector(vector3_t* vector)
{
    static int16_t count = 10;
    vector->x = count;
    vector->y = count;
    vector->z = count;
    count++;
}

// Using Carl Gauss's method
int32_t getAverageOfSumConsequentNumbers(int firstNum, int numCount)
{
    return (((firstNum + (firstNum + (numCount-1)))*(numCount/(2)))/BUFFER_SIZE);
}

/* Test Cases */

void test_accelBuffer_init_initialises_accelChip(void)
{
    // Act : init (from setUp)

    // Assert
    TEST_ASSERT_EQUAL(1, initAcclChip_fake.call_count);
}


void test_accel_average_for_each_buffer_is_the_same(void)
{
    getAcclData_fake.custom_fake = writeStuffToVector;
    // Arrange given full buffer
    for (int i = 0; i < (BUFFER_SIZE / 2); i++)
    {
        pollAccelData();
    }

    // Act : read at same time
    vector3_t vector = getAverageAccel();

    // Assert average be the same meaning they are read at same time
    // 10 + 11 + 12 + 13 + 14 + 15 = 65 / 20 = 3

    int32_t num = getAverageOfSumConsequentNumbers(10,BUFFER_SIZE/2);

    TEST_ASSERT_EQUAL(num, vector.x);
    TEST_ASSERT_EQUAL(num, vector.y);
    TEST_ASSERT_EQUAL(num, vector.z);
}

void test_accel_read_at_same_time(void)
{
    getAcclData_fake.custom_fake = writeStuffToVector;
    // Arrange : given accelerometer buffer with data and read some amount of time
    pollAccelData();
    getAverageAccel();
    pollAccelData();
    getAverageAccel();
    pollAccelData();
    getAverageAccel();
    pollAccelData();
    // Act : Read from AccelBuf
    vector3_t vector = getAverageAccel();
    // Assert
    int32_t num = getAverageOfSumConsequentNumbers(10,4);

    // 10 + 11 + 12 + 14 = 47 / 20 = 2
    TEST_ASSERT_EQUAL(num, vector.x);
    TEST_ASSERT_EQUAL(num, vector.x);
    TEST_ASSERT_EQUAL(num, vector.x);
}


void test_accel_write_at_same_time(void)
{
    getAcclData_fake.custom_fake = writeStuffToVector;
    // Arrange : given empty accelerometer buffer
    // Act : Write to buffer
    pollAccelData();
    pollAccelData();
    // ASSERT : all buffers in AccelBuf have values at the same time use average
    vector3_t vector = getAverageAccel();

    int32_t num = getAverageOfSumConsequentNumbers(10,2);
    TEST_ASSERT_EQUAL(num, vector.x);
    TEST_ASSERT_EQUAL(num,vector.y);
    TEST_ASSERT_EQUAL(num,vector.z);
}


