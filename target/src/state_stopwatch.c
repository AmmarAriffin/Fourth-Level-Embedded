#include "state.h"
#include "displayInterface.h"

#include "stopwatch.h"

/* Include what it will transition into */
#include "state_distance.h"
#include "state_timer.h"


/* Functions for State */

static void updateDisplay(FitnessTrackerPtr context)
{
    displayTime("SW", readStopwatch(), 0, ALIGN_CENTRE, true);
    // Scrolls through existing lap times
    for (uint8_t i=0;i<3;i++) {
        if (getLapIndex() - i + 1 > 0) {
        displayNumTime("Lap ", getLapIndex() - i + 1, readLap(-i), i + 1, ALIGN_CENTRE, true);
        } else {
            clearDisplayRow(i+1);
        }
    }
}

static void resetCurrStopwatch(FitnessTrackerPtr context)
{
    resetStopwatch();
}

static void toggleStopwatch_state(FitnessTrackerPtr context)
{
    toggleStopwatch();
}

static void storeLap_state(FitnessTrackerPtr context)
{
    storeLap();
}

/* State Transitions */


static void goToTimer(FitnessTrackerPtr context)
{
    changeState(context, transitionToTimer());
}

static void goToDistance(FitnessTrackerPtr context)
{
    changeState(context, transitionToDistance());
}

/* Function when other change to this concrete state */
StatePtr transitionToStopwatch(void)
{
    static struct State startedState;
    static int initialised = 0;
    /* Prevent being initialised again in the same module */
    if (0 == initialised)
    {
        initDefaultImplementation(&startedState);
        /* Init all the functions for state */
        startedState.updateDisplay = updateDisplay;
        startedState.leftButPressed = goToTimer;
        startedState.rightButPressed = goToDistance;
        startedState.topButLongPress = resetCurrStopwatch;
        startedState.topButPressed = toggleStopwatch_state;
        startedState.botButPressed = storeLap;

        initialised = 1;
    }

    return (&startedState);
}