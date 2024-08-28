
#include "unity.h"
#include "timer_s.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations

#include "step_counter_main_mock.h"

#define NUM_TIMERS 4

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

extern timer_s *timerArray[NUM_TIMERS];

/* Unity setup and teardown */

void setUp(void)
{
    FFF_STEP_COUNTER_MAIN_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
}

void tearDown(void)
{
}

/* Test cases - toggleTimer */
void test_timer_s_initTimers_initialises_timers_correctly(void)
{
    // Arrange
    
    // Act
    initTimers();
    // Assert
    for (int i = 0;i<4;i++) {
        TEST_ASSERT_EQUAL(i+1, timerArray[i]->id);
    }
}


/* Test cases - toggleTimer */
void test_timer_s_toggleTimer_starts_timer(void)
{
    // Arrange
    initTimers();
    // Act
    toggleTimer();
    // Assert
    TEST_ASSERT_EQUAL(true, timerArray[0]->isRunning);
}

void test_timer_s_toggleTimer_stops_timer(void)
{
    // Arrange
    initTimers();
    // Act
    toggleTimer();
    toggleTimer();
    // Assert
    TEST_ASSERT_EQUAL(false, timerArray[0]->isRunning);
}

void test_timer_s_toggleTimer_starts_other_timers_with_timerCycle(void)
{
    // Arrange
    initTimers();
    // Act
    toggleTimer();
    timerCycle();
    toggleTimer();
    // Assert
    TEST_ASSERT_EQUAL(true, timerArray[1]->isRunning);
}

/* Test cases - resetTimer */
void test_timer_s_resetTimer_resets_timeRemaining(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 100;
    timerArray[0]->timeRemaining = 100;
    // Act
    resetTimer();
    // Assert
    TEST_ASSERT_EQUAL(100, timerArray[0]->timeRemaining);
}

void test_timer_s_resetTimer_resets_isRunning(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 100;
    timerArray[0]->timeRemaining = 100;
    timerArray[0]->isRunning = true;
    // Act
    resetTimer();
    // Assert
    TEST_ASSERT_EQUAL(false, timerArray[0]->isRunning);
}

/* Test cases - updateTimer */
void test_timer_s_updateTimers_reduces_timeRemaining(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 100;
    timerArray[0]->timeRemaining = 100;
    timerArray[0]->isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    // Act
    updateTimers();
    // Assert
    TEST_ASSERT_LESS_THAN(100, timerArray[0]->timeRemaining);
}

void test_timer_s_updateTimers_does_not_reduce_timeRemaining_when_not_running(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 100;
    timerArray[0]->timeRemaining = 100;
    timerArray[0]->isRunning = false;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    // Act
    updateTimers();
    // Assert
    TEST_ASSERT_EQUAL(100, timerArray[0]->timeRemaining);
}

void test_timer_s_updateTimers_ends_timer(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 1;
    timerArray[0]->timeRemaining = 1;
    timerArray[0]->isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    // Act
    toggleTimer();
    updateTimers();
    // Assert
    TEST_ASSERT_EQUAL(false, timerArray[0]->isRunning);
}

/* Test cases - readTimer */
void test_timer_s_readTimer_reduces_timeRemaining(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 100;
    timerArray[0]->timeRemaining = 100;
    timerArray[0]->isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeLeft;
    // Act
    testTimeLeft = readTimer(0);
    // Assert
    TEST_ASSERT_LESS_THAN(100, timerArray[0]->timeRemaining);
    TEST_ASSERT_LESS_THAN(100, testTimeLeft);
}

void test_timer_s_readTimer_does_not_reduce_timeRemaining_when_not_running(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 100;
    timerArray[0]->timeRemaining = 100;
    timerArray[0]->isRunning = false;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeLeft;
    // Act
    testTimeLeft = readTimer(0);
    // Assert
    TEST_ASSERT_EQUAL(100, timerArray[0]->timeRemaining);
    TEST_ASSERT_EQUAL(100, testTimeLeft);
}

void test_timer_s_readTimer_ends_timer(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 1;
    timerArray[0]->timeRemaining = 1;
    timerArray[0]->isRunning = true;
    readCurrentTick_fake.custom_fake = readCurrentTick_fake_incrementing_tick;
    uint32_t testTimeLeft;
    // Act
    toggleTimer();
    testTimeLeft = readTimer(0);
    // Assert
    TEST_ASSERT_EQUAL(false, timerArray[0]->isRunning);
}    

/* Test cases - getTimerID */
void test_timer_s_getTimerID_returns_correct_id(void)
{
    // Arrange
    
    // Act
    initTimers();
    // Assert
    TEST_ASSERT_EQUAL(1, getTimerID(0));
    TEST_ASSERT_EQUAL(2, getTimerID(1));
}  

/* Test cases - incrementTime */ 
void test_timer_s_incrementTime_increments_timeTotal_seconds(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 100;
    timerArray[0]->isRunning = false;
    // Act
    incrementTime();
    // Assert
    TEST_ASSERT_EQUAL(200, timerArray[0]->timeTotal);
}   

void test_timer_s_incrementTime_increments_timeTotal_minutes(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 6000;
    timerArray[0]->isRunning = false;
    // Act
    placeCycle();
    incrementTime();
    // Assert
    TEST_ASSERT_EQUAL(12000, timerArray[0]->timeTotal);
}  

void test_timer_s_incrementTime_increments_timeTotal_hours(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 360000;
    timerArray[0]->isRunning = false;
    // Act
    placeCycle();
    placeCycle();
    incrementTime();
    // Assert
    TEST_ASSERT_EQUAL(720000, timerArray[0]->timeTotal);
}  

/* Test cases - decrementTime */ 
void test_timer_s_decrementTime_decrements_timeTotal_seconds(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 200;
    timerArray[0]->isRunning = false;
    // Act
    decrementTime();
    // Assert
    TEST_ASSERT_EQUAL(100, timerArray[0]->timeTotal);
}   

void test_timer_s_decrementTime_decrements_timeTotal_minutes(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 12000;
    timerArray[0]->isRunning = false;
    // Act
    placeCycle();
    decrementTime();
    // Assert
    TEST_ASSERT_EQUAL(6000, timerArray[0]->timeTotal);
}  

void test_timer_s_decrementTime_decrements_timeTotal_hours(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 720000;
    timerArray[0]->isRunning = false;
    // Act
    placeCycle();
    placeCycle();
    decrementTime();
    // Assert
    TEST_ASSERT_EQUAL(360000, timerArray[0]->timeTotal);
}  

/* Test cases - setTime */ 
void test_timer_s_setTime_changes_timeTotal(void)
{
    // Arrange
    initTimers();
    timerArray[0]->timeTotal = 100;
    timerArray[0]->isRunning = false;
    // Act
    setTime(0, 200);
    // Assert
    TEST_ASSERT_EQUAL(200, timerArray[0]->timeTotal);
}   
