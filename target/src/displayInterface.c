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
#include "utils/ustdlib.h"
#include "stdio.h"
#include "OrbitOLED/OrbitOLEDInterface.h"


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
 *      Global functions
 *******************************************/
// Init the screen library
void displayInit(void)
{
    OLEDInitialise();
}


void displayValue(char* prefix, int32_t intValue, char*suffix, uint8_t row, textAlignment_t alignment, bool thousandsFormatting)
{
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs

    if (thousandsFormatting) {
        // Print a number/1000 to 3dp, with decimal point and sign
        // Use a mega cool ternary operator to decide whether to use a minus sign
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s%c%d.%03d %s", prefix, intValue<0? '-':' ', abs(intValue / 1000), abs(intValue) % 1000, suffix);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %d %s", prefix, intValue, suffix); // Can use %4d if we want uniform spacing
    }

    displayString(toDraw, row, alignment);
}

void displayString(char* inStr, uint8_t row, textAlignment_t alignment)
{
    // Get the length of the string, but prevent it from being more than 16 chars long
    uint8_t inStrLength = 0;
    while (inStr[inStrLength] != '\0' && inStrLength < DISPLAY_WIDTH) {
        inStrLength++;
    }

    // Create a 16-char long array to write to
    uint8_t i = 0;
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
    for (i = 0; i < DISPLAY_WIDTH; i++) {
        toDraw[i] = ' ';
    }
    toDraw[DISPLAY_WIDTH] = '\0'; // Set the last character to EOF

    // Set the starting position based on the alignment specified
    uint8_t startPos = 0;
    switch (alignment) {
    case ALIGN_LEFT:
        startPos = 0;
        break;
    case ALIGN_CENTRE:
        startPos = (DISPLAY_WIDTH - inStrLength) /  2;
        break;
    case ALIGN_RIGHT:
        startPos = (DISPLAY_WIDTH - inStrLength);
        break;
    }

    // Copy the string we were given onto the 16-char row
    for (i = 0; i < inStrLength; i++) {
        toDraw[i + startPos] = inStr[i];
    }

    OLEDStringDraw(toDraw, 0, row);
}

void displayTime(char* prefix, uint32_t time, uint8_t row, textAlignment_t alignment, bool milli)
{
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
    uint32_t milliSeconds = time % 100;
    time /= 100;
    uint32_t seconds = time % TIME_UNIT_SCALE;
    time /= TIME_UNIT_SCALE;
    uint32_t minutes = time % TIME_UNIT_SCALE;
    time /= TIME_UNIT_SCALE;
    uint32_t hours = time;

    if (hours == 0 && milli) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d:%02d", prefix, minutes, seconds, milliSeconds);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s %01d:%02d:%02d", prefix, hours, minutes, seconds);
    }
    displayString(toDraw, row, alignment);
}

// For when you want a number in that prefix too
void displayNumTime(char* prefix, uint8_t num, uint32_t time, uint8_t row, textAlignment_t alignment, bool milli)
{
    char toDraw[DISPLAY_WIDTH+1]; // Must be one character longer to account for EOFs
    uint32_t milliSeconds = time % 100;
    time /= 100;
    uint32_t seconds = time % TIME_UNIT_SCALE;
    time /= TIME_UNIT_SCALE;
    uint32_t minutes = time % TIME_UNIT_SCALE;
    time /= TIME_UNIT_SCALE;
    uint32_t hours = time;

    if (hours == 0 && milli) {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s%d %01d:%02d:%02d", prefix, num, minutes, seconds, milliSeconds);
    } else {
        usnprintf(toDraw, DISPLAY_WIDTH + 1, "%s%d %01d:%02d:%02d", prefix, num, hours, minutes, seconds);
    }

    displayString(toDraw, row, alignment);
}


void displayCursor(uint8_t place, uint8_t row, textAlignment_t alignment)
{
    switch(place) {
        case 0:
            displayString("             ^S", row, alignment);
            break;          
        case 1:
            displayString("          ^M   ", row, alignment);
            break;
        case 2:
            displayString("       ^H      ", row, alignment);
            break;
    }
}


void clearDisplayRow(uint8_t row)
{
    char* emptyLine = "                ";
    OLEDStringDraw(emptyLine, 0, row);
}


void clearDisplay(void)
{
    char* emptyLine = "                ";
    OLEDStringDraw(emptyLine, 0, 0);
    OLEDStringDraw(emptyLine, 0, 1);
    OLEDStringDraw(emptyLine, 0, 2);
    OLEDStringDraw(emptyLine, 0, 3);
}