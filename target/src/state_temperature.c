#include "state_temperature.h"
#include "displayInterface.h"

#include "temp_measure.h"
#include "unitConverter.h"

/* Include what it will transition into */
#include "state_distance.h"



/* Functions for State */

static void updateDisplay(FitnessTrackerPtr context)
{
    pollTemp();
    uint32_t currTemp = getTemp();
 
    /* First Line */
    displayString("", FIRST_ROW, ALIGN_LEFT);


    /* Second Line */

    displayString("Curr Temp", SECOND_ROW, ALIGN_CENTRE);

    /* Third Line*/
    // If workout starts 
    if (context->secondsElapsed != 0)
    {
        switch (getUnit())
        {
            case UNIT_IMPERIAL:
                displayValue("", changeToFahrenheit(currTemp), "F", THIRD_ROW, ALIGN_CENTRE);
                break;
            default:
                displayValue("", currTemp, "C", THIRD_ROW, ALIGN_CENTRE);
                break;
        }
    } else {
        displayString("Workout has not started", THIRD_ROW, ALIGN_CENTRE);
    }

    /* Fourth Line */
    displayTime("Time:", context->secondsElapsed, FOURTH_ROW, ALIGN_CENTRE);
    
}

static void changeUnits(FitnessTrackerPtr context)
{
    incrementUnitType();
}

/* State Transitions */

static void goToDistance(FitnessTrackerPtr context)
{
    changeState(context, transitionToDistance());
}

/* Function when other change to this concrete state */
StatePtr transitionToTemperature(void)
{
    static struct State startedState;
    static int initialised = 0;
    /* Prevent being initialised again in the same module */
    if (0 == initialised)
    {
        initDefaultImplementation(&startedState);
        initTempADC();
        /* Init all the functions for state */
        startedState.rightButPressed = goToDistance;
        startedState.updateDisplay = updateDisplay;
        startedState.topButPressed = changeUnits;

        initialised = 1;
    }

    return (&startedState);
}