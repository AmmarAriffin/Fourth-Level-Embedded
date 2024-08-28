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

#define LONG_PRESS_CYCLES 35





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

    topButHandler(instance);
    botButHandler(instance);

    // botButLongPress(instance);
    // topButLongPress(instance);

    // // Ensure short press is after long press is called
    // botButPressed(instance);
    // topButPressed(instance);
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

void topButHandler(FitnessTrackerPtr instance)
{
    static bool topLongPressTriggered = 0;
    static uint16_t longPressCountTop = 0;
    if (isDown(UP)) {
        checkButton(UP);
        longPressCountTop++;
        if (longPressCountTop >= LONG_PRESS_CYCLES && !topLongPressTriggered) {
            instance->state->topButLongPress(instance);
            topLongPressTriggered = 1;
        } 
    } else if (checkButton(UP) == RELEASED && longPressCountTop < LONG_PRESS_CYCLES) {
        instance->state->topButPressed(instance);
        longPressCountTop = 0;
    } else if (checkButton(UP) == NO_CHANGE ) {
        longPressCountTop = 0;   
        topLongPressTriggered = 0;
    }

}

void botButHandler(FitnessTrackerPtr instance)
{
    static bool botLongPressTriggered = 0;
    static uint16_t longPressCountBot = 0;

    if (isDown(DOWN)) {
        checkButton(DOWN);
        longPressCountBot++;
        if (longPressCountBot >= LONG_PRESS_CYCLES && !botLongPressTriggered) {
            instance->state->botButLongPress(instance);
            botLongPressTriggered = 1;
        } 
    } else if (checkButton(DOWN) == RELEASED && longPressCountBot < LONG_PRESS_CYCLES) {
        instance->state->botButPressed(instance);
        longPressCountBot = 0;
    } else if (checkButton(DOWN) == NO_CHANGE ) {
        longPressCountBot = 0;   
        botLongPressTriggered = 0;
    }
}

void leftSwitchON(FitnessTrackerPtr instance)
{
    if (isSwitchUp()) {
        instance->state->leftSWPressed(instance);
    }
}

/* To be used by concrete states to change states*/
void changeState(FitnessTrackerPtr instance, StatePtr newState)
{
    instance->state = newState; // Store address of newState 
}

