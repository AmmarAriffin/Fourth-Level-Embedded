/*
 * Step_Counter_Main.c
 *
 *  Created on: 23/03/2022
 *      Authors: Matthew Suter, Daniel Rabbidge, Timothy Preston-Marshall
 *
 *  Main code for the ENCE361 step counter project
 *
 *  FitnessThur9-1
 */

// Comment this out to disable serial plotting
// #define SERIAL_PLOTTING_ENABLED


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



#ifdef SERIAL_PLOTTING_ENABLED
#include "serial_sender.h"
#endif //SERIAL_PLOTTING_ENABLED

#include "displayInterface.h"

#include "step_counter_main.h"

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
#define FLASH_MESSAGE_TIME 3/2 // seconds


#ifdef SERIAL_PLOTTING_ENABLED
#define RATE_SERIAL_PLOT_HZ 100
#endif // SERIAL_PLOTTING_ENABLED


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

deviceStateInfo_t deviceState; // Stored as one global so it can be accessed by other helper libs within this main module

/***********************************************************
 * Initialisation functions
 ***********************************************************/
void initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
}

/***********************************************************
 * Helper functions
 ***********************************************************/

// Flash a message onto the screen, overriding everything else
void flashMessage(char* toShow)
{
    deviceState.flashTicksLeft = RATE_DISPLAY_UPDATE_HZ * FLASH_MESSAGE_TIME;

    uint8_t i = 0;
    while (toShow[i] != '\0' && i < MAX_STR_LEN) {
        (deviceState.flashMessage)[i] = toShow[i];

        i++;
    }

    deviceState.flashMessage[i] = '\0';
}


// error trapping function that FreeRTOS
void vAssertCalled( const char * pcFile, unsigned long ulLine ) {
 (void)pcFile; // unused
 (void)ulLine; // unused
 while (true);
}

void superloop(void* args) 
{
    
    // Variable Declarations
    unsigned long lastIoProcess= 0;
    unsigned long lastAcclProcess = 0;
    unsigned long lastDisplayProcess = 0;

    #ifdef SERIAL_PLOTTING_ENABLED
    unsigned long lastSerialProcess = 0;
    #endif // SERIAL_PLOTTING_ENABLED

    uint8_t stepHigh = false;

    // Device state
    // Omnibus struct that holds loads of info about the device's current state, so it can be updated from any function
    deviceState.displayMode = DISPLAY_STEPS;
    deviceState.currentGoal = TARGET_DISTANCE_DEFAULT;
    deviceState.debugMode = false;
    deviceState.displayUnits = UNITS_SI;
    deviceState.displaySteps = STEPS_TOTAL;
    deviceState.workoutStartTick = 0;
    deviceState.currentTemp = 0;

    // Init libs
    initClock();
    initAccelBuffer(); // init buffer and accel chip
    initTimers();
    #ifdef SERIAL_PLOTTING_ENABLED
    SerialInit ();
    #endif // SERIAL_PLOTTING_ENABLED

    /* State Pattern */
    static struct FitnessTracker fitnessTracker = {0,0};

    FitnessTrackerPtr fitnessTrackerPtr = &fitnessTracker;
    
    fitnessTracker.timeElapsed = 0;
    // Start state
    startTracker(fitnessTrackerPtr);

    while(1)
    {
        unsigned long currentTick = readCurrentTick();
        
        // Poll the buttons and potentiometer
        if (lastIoProcess + RATE_SYSTICK_HZ/RATE_IO_HZ < currentTick) {
            lastIoProcess = currentTick;

            pollGPIO(fitnessTrackerPtr);

        }

        // Read and process the accelerometer
        if (lastAcclProcess + RATE_SYSTICK_HZ/RATE_ACCL_HZ < currentTick) {
            lastAcclProcess = currentTick;

            pollAccelData();

            vector3_t mean = getAverageAccel();

            uint16_t combined = sqrt(mean.x*mean.x + mean.y*mean.y + mean.z*mean.z);

            if (combined >= STEP_THRESHOLD_HIGH && stepHigh == false) {
                stepHigh = true;
                incrementStep();

                // flash a message if the user has reached their goal
                if (getStepsCount() == deviceState.currentGoal) {
                    flashMessage("Goal reached!");
                }

            } else if (combined <= STEP_THRESHOLD_LOW) {
                stepHigh = false;
            }

            // Don't start the workout until the user begins walking
            if (getStepsCount() == 0) {
                deviceState.workoutStartTick = currentTick;
            }
        }

        // Write to the display
        if (lastDisplayProcess + RATE_SYSTICK_HZ/RATE_DISPLAY_UPDATE_HZ < currentTick) {
            lastDisplayProcess = currentTick;

            uint16_t timeElapsed = (currentTick - deviceState.workoutStartTick)/TICK_MOD;
            fitnessTracker.timeElapsed = timeElapsed;
            display(fitnessTrackerPtr);
        }

        // Send to USB via serial
        #ifdef SERIAL_PLOTTING_ENABLED
        if (lastSerialProcess + RATE_SYSTICK_HZ/RATE_SERIAL_PLOT_HZ < currentTick) {
            lastSerialProcess = currentTick;

            SerialPlot(getStepsCount(), mean.x, mean.y, mean.z);
        }
        #endif // SERIAL_PLOTTING_ENABLED



        // Protection in the unlikely case the device is left running for long enough for the system tick counter to overflow
        // This prevents the last process ticks from being 'in the future', which would prevent the update functions from being called,
        // rendering the device inoperable.
        // This would take ~49 days, but is not impossible if the user forgets to turn it off before they put it away (assuming th battery lasts that long)
        if (currentTick < lastIoProcess) {
            lastIoProcess = 0;
        }

        if (currentTick < lastAcclProcess) {
            lastAcclProcess = 0;
        }

        if (currentTick < lastDisplayProcess) {
            lastDisplayProcess = 0;
        }

        #ifdef SERIAL_PLOTTING_ENABLED
        if (currentTick < lastSerialProcess) {
            lastSerialProcess = 0;
        }
        #endif // SERIAL_PLOTTING_ENABLED
    }

}


//  * Main Loop
//  ***********************************************************/

int main(void)
{
    // Fitness Monitor 1.0 Initiialisation
    
    // Init libs
    initClock();
    displayInit();
    btnInit();
    acclInit();
    initADC();
    initTempADC();


    xTaskCreate(&superloop, "superloop", 512, NULL, 1, NULL);
    vTaskStartScheduler();

    return 0; // Should never reach here

}
