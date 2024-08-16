#include "unity.h"
#include "displayInterface.h"

#include "fff.h"
DEFINE_FFF_GLOBALS;
#define FFF_MOCK_IMPL // Includes mock implementations
#include "OrbitOLED_mock.h"

static const char* capturedString = NULL;

void getString(const char* str, uint32_t x, uint32_t y)
{
    capturedString = str;
}


void setUp(void)
{
    FFF_ORBIT_FAKES_LIST(RESET_FAKE);
    FFF_RESET_HISTORY();
    capturedString = NULL;
    OLEDStringDraw_fake.custom_fake = getString;
}

void tearDown(void)
{

}

#define DISPLAY_WIDTH 16+1

/* Test Functions */

void test_string_length_always_displayWidth(void)
{
    // Arrange 
    char* test = "test";


    // Act
    displayString(test, 0,ALIGN_LEFT);

    

    // Assert
    TEST_ASSERT_EQUAL(DISPLAY_WIDTH, strlen(capturedString));
}


void test_align_string_at_centre_correctly(void)
{
    // Arrange
    char* test = "test";
    int len = strlen(test);

    // Act
    displayString(test, 0, ALIGN_CENTRE);
    // Assert
    TEST_ASSERT_EQUAL("t", capturedString[(DISPLAY_WIDTH - len)/2]);
    TEST_ASSERT_EQUAL("t", capturedString[(DISPLAY_WIDTH - len)]);
}




