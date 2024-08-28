
#include "state_distance.h"
#include "displayInterface.h"
#include <stdint.h>

#include "stepCounter.h"
#include "unitConverter.h"

#include "state_setGoal.h"
#include "state_stopwatch.h"


static bool isDebugging = false;
/* Functions for State */
static void updateDisplay(FitnessTrackerPtr context)
{   
    
    /* First Line */
    displayString("", FIRST_ROW, ALIGN_CENTRE);

    /* Second Line *//* Third Line */

    switch (getUnit()) 
    {
        case UNIT_STEPS:
            displayValue("", getStepsCount() , "steps", SECOND_ROW, ALIGN_CENTRE, false);
            displayValue("", getStepsPercent() , "% of goal", THIRD_ROW, ALIGN_CENTRE, false);
            break;
        case UNIT_METRIC:
            displayValue("Dist:", changeToKM(getStepsCount()), "km", SECOND_ROW, ALIGN_CENTRE, true);
            displayValue("Speed:", changeToKPH(getStepsCount(), context->timeElapsed), "kph", THIRD_ROW, ALIGN_CENTRE, true);
            break;
        case UNIT_IMPERIAL:
            displayValue("Dist:", changeToMiles(getStepsCount()), "mi", SECOND_ROW, ALIGN_CENTRE, true);
            displayValue("Speed:", changeToMPH(getStepsCount(), context->timeElapsed), "mph", THIRD_ROW, ALIGN_CENTRE, true);
            break;
        default: 
            break;
    }

    /* Fourth Line */
    displayTime("Time:", context->timeElapsed, FOURTH_ROW, ALIGN_CENTRE, false);
}

static void changeUnits(FitnessTrackerPtr context)
{
    if (isDebugging){
        incrementStep45();
    } else {
        incrementUnitType();
    }
}

static void decrementSteps(FitnessTrackerPtr context)
{
    if (isDebugging){
        decrementStep45();
    }
}

static void resetSteps(FitnessTrackerPtr context)
{
    if (!isDebugging) {
        resetStepCount();
    }
        
}

static void debugModeOn(FitnessTrackerPtr context)
{
    isDebugging = true;
}

static void debugModeOff(FitnessTrackerPtr context)
{
    isDebugging = false;
}

/* State Transitions */

static void goToSetGoal(FitnessTrackerPtr context)
{
    changeState(context, transitionToSetGoal());
}

static void goToStopwatch(FitnessTrackerPtr context)
{
    changeState(context, transitionToStopwatch());
}

/* Function for coming into this module */
StatePtr transitionToDistance(void)
{
    static struct State startedState;
    static int initialised = 0;
    /* Prevent being initialised again in the same module */
    if (0 == initialised)
    {

        initDefaultImplementation(&startedState);
        initStepCounter();
        
        /* Init all the functions for state */
        startedState.rightButPressed = goToStopwatch;
        startedState.updateDisplay = updateDisplay;
        startedState.topButPressed = changeUnits;
        startedState.botButLongPress = resetSteps;
        startedState.botButPressed = decrementSteps;
        startedState.leftButPressed = goToSetGoal;
        startedState.rightSWOn = debugModeOn;
        startedState.rightSWOff = debugModeOff;

        initialised = 1;
    }

    return (&startedState);
}