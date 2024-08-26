#include "state_timer.h"
#include "displayInterface.h"
#include "timer_s.h"

#include <stdbool.h>

/* Include what it will transition into */
#include "state_timer.h"

/* Functions for State */

static void updateDisplay(FitnessTrackerPtr context)
{
    displayString("", 0, ALIGN_CENTRE);
    displayNumTime("-> T ", getTimerID(getSelectedTimer()), readTimer(getSelectedTimer()), 1, ALIGN_CENTRE, false);
    displayCursor(getSelectedPlace(), 2, ALIGN_CENTRE);
    displayString("", 3, ALIGN_CENTRE);
}

static void selectTimeFormat(FitnessTrackerPtr context)
{
    // Select the place time to change which is 
    // hours, minutes and seconds
    placeCycle();
}

static void selectTimerToChange(FitnessTrackerPtr context)
{
    timerCycle();
}

static void incrementTime_state(FitnessTrackerPtr context)
{
    incrementTime();
}

static void decrementTime_state(FitnessTrackerPtr context)
{
    decrementTime();
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
        startedState.leftButPressed = incrementTime_state;
        startedState.rightButPressed = decrementTime_state;
        


        initialised = 1;
    }

    return (&startedState);
}