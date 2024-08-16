#include "stdint.h"
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"
#include "buttons4.h"
#include "switches.h"
#include "Fitness_Tracker.h"

/* Include what it will transition into */
#include "state_distance.h"

#define LONG_PRESS_CYCLES 20

void startTracker(FitnessTrackerPtr instance)
{
    StatePtr test = transitionToDistance();
    changeState(instance, test);
    initButtons();
    initSwitch();
}

void pollGPIO(FitnessTrackerPtr instance)
{
    updateButtons();
    updateSwitch();
    /* All Buttons */
    rightButPressed(instance);
    leftButPressed(instance);
    topButPressed(instance);
    botButPressed(instance);
    leftSwitchON(instance);
    botButLongPress(instance);
}


/* State-Specific Functions */

// ====================================================================================
// Display Stuff
// ====================================================================================

void display(FitnessTrackerPtr instance)
{
    instance->state->updateDisplay(instance);
}



// ====================================================================================
// Button Stuff
// ====================================================================================
void rightButPressed(FitnessTrackerPtr instance)
{
    if (checkButton(RIGHT) == PUSHED) {
        instance->state->rightButPressed(instance);
    }
}

void leftButPressed(FitnessTrackerPtr instance)
{
    if (checkButton(LEFT) == PUSHED) {
        instance->state->leftButPressed(instance);
    }
}

void topButPressed(FitnessTrackerPtr instance)
{
    if (checkButton(UP) == PUSHED) {
        instance->state->topButPressed(instance);
    }
}

void botButPressed(FitnessTrackerPtr instance)
{
    if (checkButton(DOWN) == PUSHED) {
        instance->state->botButPressed(instance);
    }
}
void leftSwitchON(FitnessTrackerPtr instance)
{
    if (isSwitchUp()) {
        instance->state->leftSWPressed(instance);
    }
}

void botButLongPress(FitnessTrackerPtr instance)
{
    static uint16_t longPressCount = 0;

    if (isDown(DOWN)) {
        longPressCount++;
        if (longPressCount >= LONG_PRESS_CYCLES) {
            instance->state->botButLongPress(instance);
        }
    }
}

/* To be used by concrete states to change states*/
void changeState(FitnessTrackerPtr instance, StatePtr newState)
{
    instance->state = newState; // Store address of newState 
}

