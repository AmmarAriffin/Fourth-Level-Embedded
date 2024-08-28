#include "unity.h"
#include "unitConverter.h"


void setUp(void)
{
    while (getUnit() != UNIT_STEPS)
    {
        incrementUnitType;
    }
}

void tearDown(void)
{
}


void test_changeToMiles_is_correct(void)
{
    // Arrange

    // Act
    uint32_t val = changeToMiles(1000);
    // Assert
    TEST_ASSERT_EQUAL(558, val);
}


void test_changeToKM_is_correct(void)
{
    // Arrange

    // Act
    uint32_t val = changeToKM(1000);
    // Assert
    TEST_ASSERT_EQUAL(900, val);
}

void test_changeToFahrenheit_is_correct(void)
{
    // Arrange

    // Act
    uint32_t val = changeToFahrenheit(1000);
    // Assert
    TEST_ASSERT_EQUAL(1832, val);
}

void test_changeToMPH_is_correct(void)
{
    // Arrange

    // Act
    uint32_t val = changeToMPH(1000, 2);
    // Assert
    TEST_ASSERT_EQUAL(279, val);
}

void test_changeToKPH_is_correct(void)
{
    // Arrange

    // Act
    uint32_t val = changeToKPH(1000, 10);
    // Assert
    TEST_ASSERT_EQUAL(324, val);
}

void test_increment_goes_back_to_start(void)
{
    // Arrange
    incrementUnitType();
    incrementUnitType();
    incrementUnitType();
    
    // Act
    UnitType unit = getUnit();
    // Assert
    TEST_ASSERT_EQUAL(UNIT_STEPS, unit);
}