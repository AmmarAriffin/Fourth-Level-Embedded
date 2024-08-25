#include "state_timer.h"
#include "displayInterface.h"

#include <stdbool.h>

/* Include what it will transition into */
#include "state_timer.h"

/* Functions for State */

static void updateDisplay(FitnessTrackerPtr context)
{
    
}

static void selectTimeFormat(FitnessTrackerPtr context)
{
    // Select the place time to change which is 
    // hours, minutes and seconds
}

static void selectTimerToChange(FitnessTrackerPtr context)
{
    //
}


/* State Transitions */

static void goBackToTimer(FitnessTrackerPtr context)
{
    changeState(context, transitionToTimer());
}


/* Function when other change to this concrete state */
StatePtr transitionToSetTimer(void)
{
    static struct State startedState;
    static int initialised = 0;
    /* Prevent being initialised again in the same module */
    if (0 == initialised)
    {
        initDefaultImplementation(&startedState);
        /* Init all the functions for state */
        startedState.updateDisplay = updateDisplay;
        startedState.topButPressed = selectTimeFormat;
        startedState.botButLongPress = goBackToTimer;
        startedState.botButPressed = selectTimerToChange;


        initialised = 1;
    }

    return (&startedState);
}