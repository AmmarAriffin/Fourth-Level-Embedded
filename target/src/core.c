// *******************************************************
// 
// core.c
//
// Author: S Fernando
// 
// Description: Contains core functions/variables which are required by many modules.

// *******************************************************

#include <stdbool.h>
#include "core.h"
#include "displayInterface.h"

#include "FreeRTOS.h"
#include "task.h"


#define RATE_DISPLAY_UPDATE_HZ 5
#define FLASH_MESSAGE_TIME 3/2 // seconds


static unsigned long currentTick = 0;

static char flashText[MAX_STR_LEN + 1];
static unsigned long flashTicksLeft;  
static bool flashing = false;


unsigned long readCurrentTick(void)
{
    currentTick = xTaskGetTickCount();
    return currentTick;
}


void flashMessage(char* toShow)
{
    flashTicksLeft = RATE_DISPLAY_UPDATE_HZ * FLASH_MESSAGE_TIME;

    uint8_t i = 0;
    while (toShow[i] != '\0' && i < MAX_STR_LEN) {
        flashText[i] = toShow[i];
        i++;
    }
    flashing = true;
}


void displayFlashMessage(void)
{
    if (flashTicksLeft > 0){
        flashTicksLeft--;
        char* emptyLine = "                ";
        displayString(emptyLine, 0, ALIGN_CENTRE);
        displayString(flashText, 1, ALIGN_CENTRE);
        displayString(emptyLine, 2, ALIGN_CENTRE);
        displayString(emptyLine, 3, ALIGN_CENTRE);
    } else {
        uint8_t i = 0;
        while (flashText[i] != '\0' && i < MAX_STR_LEN) {
            flashText[i] = '\0';
            i++;
        }
        flashing = false;
    }
}


bool isFlashing(void)
{
    return flashing;
}