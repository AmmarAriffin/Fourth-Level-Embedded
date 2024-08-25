#include "state_timer.h"
#include "displayInterface.h"

#include <stdbool.h>
#include "timer_s.h"

/* Include what it will transition into */
#include "state_stopwatch.h"
#include "state_temperature.h"

static bool isSetTimer = 0;

/* Functions for State */

static void updateDisplay(FitnessTrackerPtr context)
{

}


static void startSelectedTimer(FitnessTrackerPtr context)
{
    
}

static void goToNextTimer(FitnessTrackerPtr context)
{
    timerSelect = (timerSelect++) % NUM_TIMERS;
}


/* State Transitions */

static void goToSetTimer(FitnessTrackerPtr context)
{
    changeState(context, transitionToSetTimer());
}

static void goToTemperature(FitnessTrackerPtr context)
{
    changeState(context, transitionToTemperature());
}

static void goToStopwatch(FitnessTrackerPtr context)
{
    changeState(context, transitionToStopwatch());
}


/* Function when other change to this concrete state */
StatePtr transitionToTimer(void)
{
    static struct State startedState;
    static int initialised = 0;
    /* Prevent being initialised again in the same module */
    if (0 == initialised)
    {
        initDefaultImplementation(&startedState);
        /* Init all the functions for state */
        startedState.rightButPressed = goToStopwatch;
        startedState.updateDisplay = updateDisplay;
        startedState.leftButPressed = goToTemperature;
        startedState.botButPressed = goToNextTimer;
        startedState.botButLongPress = goToSetTimer;

        initialised = 1;
    }

    return (&startedState);
}