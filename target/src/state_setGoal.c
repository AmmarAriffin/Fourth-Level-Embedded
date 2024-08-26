#include "state_setGoal.h"
#include "displayInterface.h"
#include "pot_measure.h"
#include "stepCounter.h"


/* Include what it will transition into */
#include "state_temperature.h"
#include "state_distance.h"


#define STEP_GOAL_ROUNDING 100
#define POT_SCALE_COEFF 200000/4095 // in steps, adjusting to account for the potentiometer's maximum possible reading (was 20000/4095)

static uint32_t newGoal = 0;


/* Functions for State */
static void updateDisplay(FitnessTrackerPtr context)
{
    pollPot();
    /* Get New Goal */
    newGoal = getPotVal() * POT_SCALE_COEFF;
    newGoal = (newGoal / STEP_GOAL_ROUNDING) * STEP_GOAL_ROUNDING; // Round to nearest hundred
    /* First Line */
    displayString("Set Step Goal:", FIRST_ROW, ALIGN_CENTRE);

    /* Second Line*/
    displayValue("Goal:", getCurrentGoal(), "steps", SECOND_ROW, ALIGN_CENTRE);

    /* Third Line */
    if (newGoal == 0) {
        newGoal = 100;
        displayValue("New Goal:", newGoal, "", THIRD_ROW, ALIGN_CENTRE);
    } else {
        displayValue("New Goal:", newGoal, "", THIRD_ROW, ALIGN_CENTRE);
    }
    
    /* Fourth Line */
    displayTime("Time:", context->timeElapsed,  FOURTH_ROW, ALIGN_CENTRE, false);

}

void setGoalToNewGoal(FitnessTrackerPtr context)
{
    setGoal(newGoal);
}



/* State Transitions */

static void goToTemperature(FitnessTrackerPtr context)
{
    changeState(context, transitionToTemperature());
}

static void goToDistance(FitnessTrackerPtr context)
{
    changeState(context, transitionToDistance());
}

/* Function when other change to this concrete state */
StatePtr transitionToSetGoal(void)
{
    static struct State startedState;
    static int initialised = 0;
    /* Prevent being initialised again in the same module */
    if (0 == initialised)
    {
        initDefaultImplementation(&startedState);
        initPotADC();
        /* Init all the functions for state */
        startedState.rightButPressed = goToDistance;
        startedState.updateDisplay = updateDisplay;
        startedState.botButPressed = setGoalToNewGoal;
        startedState.leftButPressed = goToTemperature;


        initialised = 1;
    }

    return (&startedState);
}