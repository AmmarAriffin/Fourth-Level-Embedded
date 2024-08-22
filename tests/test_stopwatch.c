
#include "unity.h"
#include "stopwatch.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "step_counter_main_mock.h"

/* Helper functions */

unsigned long readCurrentTick_fake_incrementing_tick(void)
{
    static int count = 0;
    count += 10;
    return count;
}

// Declare the stopWatch object for testing
typedef struct
{
    uint32_t lastReadTime;
    uint32_t elapsedTime;
    uint32_t lapTimes[MAX_LAPS];
    uint8_t lapNum;
    int8_t lapReadIndex;
    bool isRunning;
} stopWatch_s;

extern stopWatch_s stopWatch;

/* Unity setup and teardown */

void setUp(void)
{
    FFF_STEP_COUNTER_MAIN_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void tearDown(void)
{
}

/* Test cases - toggleStopwatch */
void test_stopwatch_toggleStopwatch_starts_stopwatch(void)
{
    // Arrange
    // Act
    toggleStopwatch();
    // Assert
    TEST_ASSERT_EQUAL(true, stopWatch.isRunning);
}

void test_stopwatch_toggleStopwatch_stops_stopwatch(void)
{
    // Arrange
    // Act
    toggleStopwatch();
    toggleStopwatch();
    // Assert
    TEST_ASSERT_EQUAL(false, stopWatch.isRunning);
}

/* Test cases - resetStopwatch */
void test_stopwatch_resetStopwatch_resets_elapsedTime(void)
{
    // Arrange
    stopWatch.elapsedTime = 100;
    // Act
    resetStopwatch();
    // Assert
    TEST_ASSERT_EQUAL(0, stopWatch.elapsedTime);
}

void test_stopwatch_resetStopwatch_resets_isRunning(void)
{
    // Arrange
    stopWatch.isRunning = true;
    // Act
    resetStopwatch();
    // Assert
    TEST_ASSERT_EQUAL(false, stopWatch.isRunning);
}

void test_stopwatch_resetStopwatch_resets_laps(void)
{
    // Arrange
    stopWatch.isRunning = true;
    // Act
    resetStopwatch();
    // Assert
    TEST_ASSERT_EQUAL(0, stopWatch.lapNum);
    TEST_ASSERT_EQUAL(-1, stopWatch.lapReadIndex);
}

/* Test cases - readStopwatch */
void test_stopwatch_readStopwatch_increases_elapsedTime(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    // Act
    testTimeElapsed = readStopwatch();
    // Assert
    TEST_ASSERT_EQUAL(1, testTimeElapsed);
} 

void test_stopwatch_readStopwatch_does_not_increase_elapsedTime_if_not_running(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = false;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    // Act
    testTimeElapsed = readStopwatch();
    // Assert
    TEST_ASSERT_EQUAL(0, testTimeElapsed);
} 

/* Test cases - storeLap */
void test_stopwatch_storeLap_adds_a_lap_time(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    // Act
    storeLap();
    // Assert
    TEST_ASSERT_EQUAL(1, stopWatch.lapTimes[stopWatch.lapReadIndex]);
} 

void test_stopwatch_storeLap_adds_a_lap(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    // Act
    storeLap();
    // Assert
    TEST_ASSERT_EQUAL(1, stopWatch.lapNum);
} 

void test_stopwatch_storeLap_flashes_max_laps_msg(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    uint8_t  i;
    // Act
    for (i=0;i<11;i++) {
        storeLap();
    }
    // Assert
    TEST_ASSERT_EQUAL(1, flashMessage_fake.call_count);
} 

void test_stopwatch_storeLap_cycles_through_laps(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    storeLap();
    storeLap();
    // Act
    stopWatch.isRunning = false;
    stopWatch.lapReadIndex = 0;
    storeLap();
    // Assert
    TEST_ASSERT_EQUAL(1, stopWatch.lapReadIndex);
    storeLap();
    TEST_ASSERT_EQUAL(2, stopWatch.lapReadIndex);
    storeLap();
    TEST_ASSERT_EQUAL(0, stopWatch.lapReadIndex);
} 

/* Test cases - readLap */
void test_stopwatch_readLap_returns_correct_lap_time(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    // Act
    storeLap();
    storeLap();
    // Assert
    TEST_ASSERT_EQUAL(1, readLap(-1));
    TEST_ASSERT_EQUAL(2, readLap(0));
} 

void test_stopwatch_readLap_does_not_try_to_access_non_existent_laps_with_1_lap(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    // Act
    storeLap();
    // Assert
    TEST_ASSERT_EQUAL(1, readLap(0));
    TEST_ASSERT_EQUAL(1, readLap(1));
    TEST_ASSERT_EQUAL(1, readLap(-1));
    TEST_ASSERT_EQUAL(1, readLap(-99));
    TEST_ASSERT_EQUAL(1, readLap(99));
} 

void test_stopwatch_readLap_does_not_try_to_access_non_existent_laps_with_2_laps(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    // Act
    storeLap();
    storeLap();
    // Assert
    TEST_ASSERT_EQUAL(2, readLap(0));
    TEST_ASSERT_EQUAL(1, readLap(1));
    TEST_ASSERT_EQUAL(1, readLap(-1));
    TEST_ASSERT_EQUAL(2, readLap(-98));
    TEST_ASSERT_EQUAL(2, readLap(98));
} 

void test_stopwatch_readLap_does_not_try_to_access_non_existent_laps_with_10_laps(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    uint8_t i;
    // Act
    for (i=0;i<10;i++) {
        storeLap();
    }
    // Assert
    TEST_ASSERT_EQUAL(10, readLap(0));
    TEST_ASSERT_EQUAL(9, readLap(-1));
    TEST_ASSERT_EQUAL(8, readLap(-2));
    TEST_ASSERT_EQUAL(1, readLap(1));
    TEST_ASSERT_EQUAL(2, readLap(2));
    TEST_ASSERT_EQUAL(9, readLap(99));
    TEST_ASSERT_EQUAL(8, readLap(98));
    TEST_ASSERT_EQUAL(1, readLap(-99));
    TEST_ASSERT_EQUAL(10, readLap(-98));
} 

/* Test cases - getLapIndex */
void test_stopwatch_getLapIndex_returns_correct_lapReadIndex(void)
{
    // Arrange
    stopWatch.elapsedTime = 0;
    stopWatch.lastReadTime = 0;
    stopWatch.isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeElapsed;
    // Act
    storeLap();
    storeLap();
    // Assert
    TEST_ASSERT_EQUAL(1, getLapIndex());
    stopWatch.lapReadIndex = 0;
    TEST_ASSERT_EQUAL(0, getLapIndex());
} 

// properly encapsulate stopwatch
