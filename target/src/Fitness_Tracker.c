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

// I don't like having these variables here very hacky
static bool botLongPressTriggered = 0;
static bool topLongPressTriggered = 0;


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
    leftSwitchON(instance);
    botButLongPress(instance);
    topButLongPress(instance);

    // Ensure short press is after long press is called
    botButPressed(instance);
    topButPressed(instance);
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
    // RELEASED only if button has been pressed down before
    // topLongPressTriggered for ensure that short button press 
    // don't trigger after long press is triggered too
    if (checkButton(UP) == RELEASED && !topLongPressTriggered) {
        instance->state->topButPressed(instance);
        // clear up any triggers
        topLongPressTriggered = 0;
    }
}

void botButPressed(FitnessTrackerPtr instance)
{
    // Look above
    if (checkButton(DOWN) == RELEASED && !botLongPressTriggered) {
        instance->state->botButPressed(instance);
        botLongPressTriggered = 0;
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
    static uint16_t longPressCountBot = 0;

    if (isDown(DOWN)) {
        longPressCountBot++;
        if (longPressCountBot >= LONG_PRESS_CYCLES) {
            instance->state->botButLongPress(instance);
            longPressCountBot = 0;
            botLongPressTriggered = 1;
        }  
    }
}


void topButLongPress(FitnessTrackerPtr instance)
{
    static uint16_t longPressCountTop = 0;

    if (isDown(UP)) {
        longPressCountTop++;
        if (longPressCountTop >= LONG_PRESS_CYCLES) {
            instance->state->botButLongPress(instance);
            longPressCountTop = 0;
            topLongPressTriggered = 1;
        }
    }
}



/* To be used by concrete states to change states*/
void changeState(FitnessTrackerPtr instance, StatePtr newState)
{
    instance->state = newState; // Store address of newState 
}

