#include "unity.h"
#include "stepCounter.h"


void setUp(void)
{
    resetStepCount();
    setGoal(0);
}

void tearDown(void)
{
}

void test_steps_can_increment(void)
{
    // Arrange

    // Act
    incrementStep();

    // Assert
    TEST_ASSERT_EQUAL(1, getStepsCount());

}

void test_steps_can_reset(void)
{
    // Arrange
    for (int16_t i = 0; i < 5; i++) {
      incrementStep();
    }

    // Act
    resetStepCount();

    // Assert
    TEST_ASSERT_EQUAL(0, getStepsCount());
}

void test_steps_percentage_correct(void)
{
    // Arrange
    for (int16_t i = 0; i < 50; i++) {
      incrementStep();
    }
    setGoal(100);

    // Act
    uint32_t val = getStepsPercent();

    // Assert
    TEST_ASSERT_EQUAL(50, val);
}

void test_goal_can_be_set(void)
{
    // Arrange

    // Act
    setGoal(169);
    // Assert
    TEST_ASSERT_EQUAL(169, getCurrentGoal()); 
}

