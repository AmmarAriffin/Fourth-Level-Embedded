/*
 * main.c
 *
 *  Created on: 23/03/2022
 *      Authors: Matthew Suter, Daniel Rabbidge, Timothy Preston-Marshall
 *
 *  Main code for the ENCE361 step counter project
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
#include "utils/ustdlib.h"
#include "acc.h"
#include "math.h"
#include "stepCounter.h"
#include "accelerometer.h"
#include "core.h"
#include "timer_s.h"
#include "pot_measure.h"
#include "temp_measure.h"
#include "displayInterface.h"
#include "Fitness_Tracker.h"

#include "FreeRTOS.h"
#include "task.h"

/**********************************************************
 * Constants and types
 **********************************************************/
#define RATE_SYSTICK_HZ 1000
#define RATE_IO_HZ 75
#define RATE_ACCL_HZ 200
#define RATE_DISPLAY_UPDATE_HZ 5

/**********************************************************
 * Task Period Times
 **********************************************************/

#define BUTTONS_AND_POTENTIOMETER_TASK_PERIOD portTICK_PERIOD_MS * 1000 / RATE_IO_HZ
#define ACELL_TASK_PERIOD portTICK_PERIOD_MS * 1000 / RATE_ACCL_HZ
#define DISPLAY_TASK_PERIOD portTICK_PERIOD_MS * 1000 / RATE_DISPLAY_UPDATE_HZ

#define STEP_GOAL_ROUNDING 100
#define STEP_THRESHOLD_HIGH 270
#define STEP_THRESHOLD_LOW 235

#define TARGET_DISTANCE_DEFAULT 1000

#define POT_SCALE_COEFF 200000/4095 // in steps, adjusting to account for the potentiometer's maximum possible reading (was 20000/4095)

/*******************************************
 *      Local prototypes
 *******************************************/
void initClock (void);


/*******************************************
 *      Globals
 *******************************************/   

/* State Pattern */
static struct FitnessTracker fitnessTracker = {0,0};
FitnessTrackerPtr fitnessTrackerPtr = &fitnessTracker;

/***********************************************************
 * Initialisation functions
 ***********************************************************/
void initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}


// error trapping function that FreeRTOS
void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
 (void)pcFile; // unused
 (void)ulLine; // unused
 while (true);
}

void PollButtonsAndPotentiometerTask(void* args) {
    uint32_t currentTick = readCurrentTick();
    uint32_t lastIoProcess = currentTick;

    for(;;) {
        
        currentTick = readCurrentTick();
        lastIoProcess = currentTick;

        pollGPIO(fitnessTrackerPtr);
        // Poll ADC Potentiometer
        pollPot();
        getPotVal();
        // Poll ADC Temperature
        pollTemp();
        getTemp();
        
        // Protection in the unlikely case the device is left running for long enough for the system tick counter to overflow
        // This prevents the last process ticks from being 'in the future', which would prevent the update functions from being called,
        // rendering the device inoperable.
        // This would take ~49 days, but is not impossible if the user forgets to turn it off before they put it away (assuming the battery lasts that long)
        if (currentTick < lastIoProcess) {
        lastIoProcess = 0;
        }

        vTaskDelay(BUTTONS_AND_POTENTIOMETER_TASK_PERIOD);
    }

}


void AccelerometerTask(void* args) {

    uint32_t lastAcclProcess = 0;

    uint8_t stepHigh = false;
    vector3_t mean;
    uint32_t currentTick = readCurrentTick();

    for (;;) {
        currentTick = readCurrentTick();

        lastAcclProcess = currentTick;

        pollAccelData();

        mean = getAverageAccel();

        uint16_t combined = sqrt(mean.x*mean.x + mean.y*mean.y + mean.z*mean.z);

        if (combined >= STEP_THRESHOLD_HIGH && stepHigh == false) {
            stepHigh = true;
            incrementStep();

            // flash a message if the user has reached their goal
            if (getStepsCount() == getCurrentGoal()) {
                flashMessage("Goal reached!");
            }

        } else if (combined <= STEP_THRESHOLD_LOW) {
            stepHigh = false;
        }

        // Don't start the workout until the user begins walking
        if (getStepsCount() == 0) {
            fitnessTracker.workoutStartTick = currentTick;
        }

        // Protection in the unlikely case the device is left running for long enough for the system tick counter to overflow
        // This prevents the last process ticks from being 'in the future', which would prevent the update functions from being called,
        // rendering the device inoperable.
        // This would take ~49 days, but is not impossible if the user forgets to turn it off before they put it away (assuming the battery lasts that long)
        if (currentTick < lastAcclProcess) {
            lastAcclProcess = 0;
        }
 

        vTaskDelay(ACELL_TASK_PERIOD);
    }

}

void DisplayTask(void* args) {

    uint32_t currentTick = readCurrentTick();
    uint32_t lastDisplayProcess = 0;

    for(;;){
        currentTick = readCurrentTick();
        lastDisplayProcess = currentTick;

        // uint16_t secondsElapsed = (currentTick - deviceState.workoutStartTick)/RATE_SYSTICK_HZ;
        // displayUpdate(deviceState, secondsElapsed);

        uint16_t timeElapsed = (currentTick - fitnessTracker.workoutStartTick)/TICK_MOD;
        fitnessTracker.timeElapsed = timeElapsed;

        if (isFlashing()) {
            displayFlashMessage();
        } else {
            display(fitnessTrackerPtr);
        }
        
        updateTimers();

        // Protection in the unlikely case the device is left running for long enough for the system tick counter to overflow
        // This prevents the last process ticks from being 'in the future', which would prevent the update functions from being called,
        // rendering the device inoperable.
        // This would take ~49 days, but is not impossible if the user forgets to turn it off before they put it away (assuming the battery lasts that long)
        if (currentTick < lastDisplayProcess) {
            lastDisplayProcess = 0;
        }
        
        vTaskDelay(DISPLAY_TASK_PERIOD);
    }

}




void initEverything(void) {
    // Init libs
    initClock();
    initAccelBuffer(); // init buffer and accel chip
    initTempADC();
    initPotADC();
    initTimers();
    displayInit();
    initStepCounter();

    fitnessTracker.workoutStartTick = 0;
    fitnessTracker.timeElapsed = 0;
    // Start state
    startTracker(fitnessTrackerPtr);
}


//  * Main Loop
//  ***********************************************************/

int main(void)
{
    // Fitness Monitor 2.0 Initiialisation
    



    initEverything();

    xTaskCreate(&PollButtonsAndPotentiometerTask, "PollingTask", 190, NULL, 1, NULL);
    xTaskCreate(&AccelerometerTask, "AccelerometerTask", 512, NULL, 3, NULL);
    xTaskCreate(&DisplayTask, "DisplayTask", 200, NULL, 2, NULL);

    vTaskStartScheduler();

    return 0; // Should never reach here

}
