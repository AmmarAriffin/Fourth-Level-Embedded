// *******************************************************
// 
// core.c
//
// Author: S Fernando
// 
// Description: Contains core functions/variables which are required by many modules.

// *******************************************************

#include "core.h"
#include "displayInterface.h"

#include "FreeRTOS.h"
#include "task.h"


// #define RATE_DISPLAY_UPDATE_HZ 5
// #define FLASH_MESSAGE_TIME 3/2 // seconds
// #define MAX_STR_LEN 16

// For displaying temporary messages over the top of everything else
// static char flashText[MAX_STR_LEN + 1];
// static unsigned long flashTicksLeft;

unsigned long readCurrentTick(void)
{
    return xTaskGetTickCount();
}

// // TODO: make interrupt driven
// void flashMessage(char* toShow)
// {
//     flashTicksLeft = RATE_DISPLAY_UPDATE_HZ * FLASH_MESSAGE_TIME;

//     uint8_t i = 0;
//     while (toShow[i] != '\0' && i < MAX_STR_LEN) {
//         flashText[i] = toShow[i];
//         i++;
//     }

//     flashText[i] = '\0';

// }


// char* emptyLine = "                ";
// displayString(emptyLine, 0, ALIGN_CENTRE);
// displayString(flashText, 1, ALIGN_CENTRE);
// displayString(emptyLine, 2, ALIGN_CENTRE);
// displayString(emptyLine, 3, ALIGN_CENTRE);
// void TaskFunction(void *pvParameters) {
//     for (;;) {
//         // Wait for the notification
//         ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

//         // Task code to run when the function is called
//         // For example, toggle an LED, process data, etc.
//     }
// }

// void main(void) {
//     xTaskCreate(TaskFunction, "MyTask", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);
//     vTaskStartScheduler();
// }