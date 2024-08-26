
#include "state_distance.h"
#include "displayInterface.h"
#include <stdint.h>

#include "stepCounter.h"
#include "unitConverter.h"

#include "state_setGoal.h"
#include "state_stopwatch.h"



/* Functions for State */
static void updateDisplay(FitnessTrackerPtr context)
{   
    
    /* First Line */
    displayString("", FIRST_ROW, ALIGN_CENTRE);

    /* Second Line *//* Third Line */

    switch (getUnit()) 
    {
        case UNIT_STEPS:
            displayValue("", getStepsCount() , "steps", SECOND_ROW, ALIGN_CENTRE);
            displayValue("", getStepsPercent() , "% of goal", THIRD_ROW, ALIGN_CENTRE);
            break;
        case UNIT_METRIC:
            displayValue("Dist:", changeToKM(getStepsCount()), "km", SECOND_ROW, ALIGN_CENTRE);
            displayValue("Speed:", changeToKPH(getStepsCount(), context->timeElapsed), "kph", THIRD_ROW, ALIGN_CENTRE);
            break;
        case UNIT_IMPERIAL:
            displayValue("Dist:", changeToMiles(getStepsCount()), "mi", SECOND_ROW, ALIGN_CENTRE);
            displayValue("Speed:", changeToMPH(getStepsCount(), context->timeElapsed), "mph", THIRD_ROW, ALIGN_CENTRE);
            break;
        default: 
            break;
    }

    /* Fourth Line */
    displayTime("Time:", context->timeElapsed, FOURTH_ROW, ALIGN_CENTRE, false);
}

static void changeUnits(FitnessTrackerPtr context)
{
    incrementUnitType();
}


void resetSteps(FitnessTrackerPtr context)
{
    resetStepCount();    
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

        displayInit();
        /* Init all the functions for state */
        startedState.rightButPressed = goToStopwatch;
        startedState.updateDisplay = updateDisplay;
        startedState.topButPressed = changeUnits;
        startedState.botButLongPress = resetSteps;
        startedState.leftButPressed = goToSetGoal;

        initialised = 1;
    }

    return (&startedState);
}