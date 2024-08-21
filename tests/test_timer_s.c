
#include "unity.h"
#include "timer_s.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "timer_s_mock.h"

/* Helper functions */

unsigned long readCurrentTick_fake_incrementing_tick(void)
{
    static int count = 0;
    count += 10;
    return count;
}

// Expose timer_s members for testing
struct timer_s
{
    uint32_t lastReadTime;
    uint32_t timeRemaining;
    uint32_t timeTotal;
    uint8_t id;
    bool isRunning;
};

/* Unity setup and teardown */

void setUp(void)
{
    FFF_TIMER_S_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void tearDown(void)
{
}

/* Test cases - createTimer */
void test_timer_s_createTimer_creates_a_timer(void)
{
    // Arrange
    // Act
    timer_s *timerTest = createTimer(1);
    // Assert
    TEST_ASSERT_NOT_EQUAL(NULL, timerTest);
    TEST_ASSERT_EQUAL(8, sizeof(timerTest));

}

/* Test cases - destroyTimer */
void test_timer_s_destroyTimer_destroys_a_timer(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    // Act
    destroyTimer(&timerTest);
    // Assert
    TEST_ASSERT_EQUAL(NULL, timerTest);
}

/* Test cases - toggleTimer */
void test_timer_s_toggleTimer_starts_timer(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    // Act
    toggleTimer(timerTest);
    // Assert
    TEST_ASSERT_EQUAL(true, timerTest->isRunning);
}

void test_timer_s_toggleTimer_stops_timer(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    // Act
    toggleTimer(timerTest);
    toggleTimer(timerTest);
    // Assert
    TEST_ASSERT_EQUAL(false, timerTest->isRunning);
}

/* Test cases - resetTimer */
void test_timer_s_resetTimer_resets_timeRemaining(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 100;
    timerTest->timeRemaining = 100;
    // Act
    resetTimer(timerTest);
    // Assert
    TEST_ASSERT_EQUAL(100, timerTest->timeRemaining);
}

void test_timer_s_resetTimer_resets_isRunning(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 100;
    timerTest->timeRemaining = 100;
    timerTest->isRunning = true;
    // Act
    resetTimer(timerTest);
    // Assert
    TEST_ASSERT_EQUAL(false, timerTest->isRunning);
}

/* Test cases - updateTimer */
void test_timer_s_updateTimer_reduces_timeRemaining(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 100;
    timerTest->timeRemaining = 100;
    timerTest->isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    // Act
    updateTimer(timerTest);
    // Assert
    TEST_ASSERT_LESS_THAN(100, timerTest->timeRemaining);
}

void test_timer_s_updateTimer_does_not_reduce_timeRemaining_when_not_running(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 100;
    timerTest->timeRemaining = 100;
    timerTest->isRunning = false;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    // Act
    updateTimer(timerTest);
    // Assert
    TEST_ASSERT_EQUAL(100, timerTest->timeRemaining);
}

void test_timer_s_updateTimer_ends_timer(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 1;
    timerTest->timeRemaining = 1;
    timerTest->isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    // Act
    updateTimer(timerTest);
    // Assert
    TEST_ASSERT_EQUAL(0, timerTest->timeRemaining);
    TEST_ASSERT_EQUAL(false, timerTest->isRunning);
}

/* Test cases - readTimer */
void test_timer_s_readTimer_reduces_timeRemaining(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 100;
    timerTest->timeRemaining = 100;
    timerTest->isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeLeft;
    // Act
    testTimeLeft = readTimer(timerTest);
    // Assert
    TEST_ASSERT_LESS_THAN(100, timerTest->timeRemaining);
    TEST_ASSERT_LESS_THAN(100, testTimeLeft);
}

void test_timer_s_readTimer_does_not_reduce_timeRemaining_when_not_running(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 100;
    timerTest->timeRemaining = 100;
    timerTest->isRunning = false;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeLeft;
    // Act
    testTimeLeft = readTimer(timerTest);
    // Assert
    TEST_ASSERT_EQUAL(100, timerTest->timeRemaining);
    TEST_ASSERT_EQUAL(100, testTimeLeft);
}

void test_timer_s_readTimer_ends_timer(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 1;
    timerTest->timeRemaining = 1;
    timerTest->isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeLeft;
    // Act
    testTimeLeft = readTimer(timerTest);
    // Assert
    TEST_ASSERT_EQUAL(0, timerTest->timeRemaining);
    TEST_ASSERT_EQUAL(0, testTimeLeft);
    TEST_ASSERT_EQUAL(false, timerTest->isRunning);
}    

/* Test cases - getTimerID */
void test_timer_s_getTimerID_returns_correct_id(void)
{
    // Arrange
    // Act
    timer_s *timerTest1 = createTimer(1);
    timer_s *timerTest2 = createTimer(2);
    // Assert
    TEST_ASSERT_EQUAL(1, getTimerID(timerTest1));
    TEST_ASSERT_EQUAL(2, getTimerID(timerTest2));
}  

/* Test cases - incrementTime */ 
void test_timer_s_incrementTime_increments_timeTotal_seconds(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 100;
    timerTest->isRunning = false;
    // Act
    incrementTime(timerTest, 0);
    // Assert
    TEST_ASSERT_EQUAL(200, timerTest->timeTotal);
}   

void test_timer_s_incrementTime_increments_timeTotal_minutes(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 6000;
    timerTest->isRunning = false;
    // Act
    incrementTime(timerTest, 1);
    // Assert
    TEST_ASSERT_EQUAL(12000, timerTest->timeTotal);
}  

void test_timer_s_incrementTime_increments_timeTotal_hours(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 360000;
    timerTest->isRunning = false;
    // Act
    incrementTime(timerTest, 2);
    // Assert
    TEST_ASSERT_EQUAL(720000, timerTest->timeTotal);
}  

/* Test cases - decrementTime */ 
void test_timer_s_decrementTime_decrements_timeTotal_seconds(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 200;
    timerTest->isRunning = false;
    // Act
    decrementTime(timerTest, 0);
    // Assert
    TEST_ASSERT_EQUAL(100, timerTest->timeTotal);
}   

void test_timer_s_decrementTime_decrements_timeTotal_minutes(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 12000;
    timerTest->isRunning = false;
    // Act
    decrementTime(timerTest, 1);
    // Assert
    TEST_ASSERT_EQUAL(6000, timerTest->timeTotal);
}  

void test_timer_s_decrementTime_decrements_timeTotal_hours(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 720000;
    timerTest->isRunning = false;
    // Act
    decrementTime(timerTest, 2);
    // Assert
    TEST_ASSERT_EQUAL(360000, timerTest->timeTotal);
}  

/* Test cases - setTime */ 
void test_timer_s_setTime_changes_timeTotal(void)
{
    // Arrange
    timer_s *timerTest = createTimer(1);
    timerTest->timeTotal = 100;
    timerTest->isRunning = false;
    // Act
    setTime(timerTest, 200);
    // Assert
    TEST_ASSERT_EQUAL(200, timerTest->timeTotal);
}   
