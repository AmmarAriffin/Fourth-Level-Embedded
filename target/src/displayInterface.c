/*
 * display_manager.c
 *
 * Handles drawing to the device's screen, given its current state
 *
 *  Created on: 23/03/2022
 *      Author: Matthew Suter
 *
 *  FitnessThur9-1
 */


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "utils/ustdlib.h"

#include "serial_sender.h"
#include "displayInterface.h"
#include "string.h"

//********************************************************
// Constants and static vars
//********************************************************

#define KM_TO_MILES 62/100 // Multiply by 0.6215 to convert, this should be good enough
#define MS_TO_KMH 36/10
#define TIME_UNIT_SCALE 60

#define DISPLAY_WIDTH 16
/*******************************************
 *      Local prototypes
 *******************************************/
static void displayLine(char* inStr, uint8_t row, textAlignment_t alignment);
static void displayValue(char* prefix, char* suffix, int32_t value, uint8_t row, textAlignment_t alignment, bool thousandsFormatting);
static void displayTime(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment);


/*******************************************
 *      Helper Functions
 *******************************************/

static uint8_t padString(char* str, int startPos, int endPos)
{
    for (int i = startPos; i < endPos; i++) {
        str[i] = ' ';
    }
}

static void fitStringForDisplayWidth(char* str, textAlignment_t alignment)
{
    int len = strlen(str);

    // Pad with empty spaces to match num chars to be DISPLAY_WIDTH
    if (len < DISPLAY_WIDTH) {
        int totalPadding = DISPLAY_WIDTH - len;

        switch (alignment) {
            case ALIGN_LEFT:
                padString(str, len, totalPadding);
            case ALIGN_CENTRE:

                int leftPadding = totalPadding/2;
                int rightPadding = totalPadding - leftPadding;

                memmove(str + leftPadding, str, len+1);

                padString(str, 0, leftPadding); // Pad front
                padString(str, len+leftPadding, DISPLAY_WIDTH); // Pad back
            case ALIGN_RIGHT:
                memmove(str + totalPadding, str, len+1);
                padString(str, 0, totalPadding);
        }   
    }
    // Would automatically truncate if char width longer than DISPLAY WIDTH
    str[DISPLAY_WIDTH] = '\0';
}

/*******************************************
 *      Global functions
 *******************************************/
// Init the screen library
void displayInit(void)
{
    OLEDInitialise();
}


void displayValue(char* prefix, int32_t intValue, char*suffix, uint8_t row, textAlignment_t alignment)
{
    char toDraw[DISPLAY_WIDTH+1];
    usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %d %s", prefix, intValue, suffix);
    OLEDStringDraw(toDraw, 0, row);
}

void displayString(char* inStr, uint8_t row, textAlignment_t alignment)
{
    fitStringForDisplayWidth(inStr, alignment);
    OLEDStringDraw(inStr, 0, row);
}

// I wanna get rid of this
void displayTime(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment)
{
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
    uint16_t minutes = (time / TIME_UNIT_SCALE) % TIME_UNIT_SCALE;
    uint16_t seconds = time % TIME_UNIT_SCALE;
    uint16_t hours =   time / (TIME_UNIT_SCALE * TIME_UNIT_SCALE);

    if (hours == 0) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d", prefix, minutes, seconds);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d:%02d", prefix, hours, minutes, seconds);
    }
    OLEDStringDraw(toDraw, 0, row);
}

void clearDisplay()
{
    char* emptyLine = "                ";
    OLEDStringDraw(emptyLine, 0, 0);
    OLEDStringDraw(emptyLine, 0, 1);
    OLEDStringDraw(emptyLine, 0, 2);
    OLEDStringDraw(emptyLine, 0, 3);
}




















// Update the display, called on a loop
void displayUpdate(deviceStateInfo_t deviceState, uint16_t secondsElapsed)
{
    // Check for flash message override
    if (deviceState.flashTicksLeft != 0) {
        char* emptyLine = "                ";
        OLEDStringDraw (emptyLine, 0, 0);
        displayLine(deviceState.flashMessage, 1, ALIGN_CENTRE);
        OLEDStringDraw (emptyLine, 0, 2);
        OLEDStringDraw (emptyLine, 0, 3);
        return;
    }


    uint32_t mTravelled = 0; // TODO: If I put this inside the case statement it won't compile. Work out why!

    switch (deviceState.displayMode) {
    case DISPLAY_STEPS:
        displayLine("", 0, ALIGN_CENTRE); // Clear the top line
        if (deviceState.displayUnits == UNITS_SI) {
            displayValue("", "steps", deviceState.stepsTaken, 1, ALIGN_CENTRE, false);
        } else {
            displayValue("", "% of goal", deviceState.stepsTaken * 100 / deviceState.currentGoal, 1, ALIGN_CENTRE, false);
        }
        displayTime("Time:", secondsElapsed, 2, ALIGN_CENTRE);
        break;
    case DISPLAY_DISTANCE:
        displayTime("Time:", secondsElapsed, 1, ALIGN_CENTRE);
        mTravelled = deviceState.stepsTaken * M_PER_STEP;

        // Protection against division by zero
        uint16_t speed;
        if (secondsElapsed != 0) {
            speed = (mTravelled / secondsElapsed) * MS_TO_KMH; // in km/h
        } else {
            speed = mTravelled * MS_TO_KMH; // if zero seconds elapsed, act as if it's at least one
        }

        if (deviceState.displayUnits == UNITS_SI) {
            displayValue("Dist:", "km", mTravelled, 0, ALIGN_CENTRE, true);
            displayValue("Speed", "kph", speed, 2, ALIGN_CENTRE, false);
        } else {
            displayValue("Dist:", "mi", mTravelled * KM_TO_MILES, 0, ALIGN_CENTRE, true);
            displayValue("Speed", "mph", speed * KM_TO_MILES, 2, ALIGN_CENTRE, false);
        }

        break;
    case DISPLAY_TEMPERATURE:
        if (deviceState.workoutStartTick != 0)
        {
            displayValue("Temp", "deg", deviceState.currentTemp, 0, ALIGN_CENTRE, false);
        } else {
            displayLine("Temp: Workout not started", 0, ALIGN_CENTRE);
        }
        displayValue("", "steps", deviceState.stepsTaken, 1, ALIGN_CENTRE, false);
        displayTime("Time:", secondsElapsed, 2, ALIGN_CENTRE);
        break;
    case DISPLAY_SET_GOAL:
        displayLine("Set goal:", 0, ALIGN_CENTRE);
        displayValue("Current:", "", deviceState.currentGoal, 2, ALIGN_CENTRE, false);

        // Display the step/distance preview
        char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
        uint16_t distance = deviceState.newGoal * M_PER_STEP;
        if (deviceState.displayUnits != UNITS_SI) {
            distance = distance * KM_TO_MILES;
        }

        // if <10 km/miles, use a decimal point. Otherwise display whole units (to save space)
        if (distance < 10*1000) {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d.%01d%s", deviceState.newGoal, distance / 1000, (distance % 1000)/100, deviceState.displayUnits == UNITS_SI ? "km" : "mi");
        } else {
            usnprintf(toDraw, DISPLAY_WIDTH + 1, "%d stps/%d%s", deviceState.newGoal, distance / 1000, deviceState.displayUnits == UNITS_SI ? "km" : "mi");
        }

        displayLine(toDraw, 1, ALIGN_CENTRE);

        break;
    }
}

