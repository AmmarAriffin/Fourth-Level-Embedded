#include "state_temperature.h"
#include "displayInterface.h"

#include "temp_measure.h"
#include "unitConverter.h"

/* Include what it will transition into */
#include "state_distance.h"

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

        initialised = 1;
    }

    return (&startedState);
}

/* Functions for State */

void updateDisplay(FitnessTrackerPtr context)
{
    pollTemp();
    uint32_t currTemp = getTemp();

    /* First Line */
    displayString("", FIRST_ROW, ALIGN_LEFT);


    /* Second Line */

    displayString("Ambient Temperature", SECOND_ROW, ALIGN_CENTRE);

    /* Third Line*/
    // If workout starts 
    if (context->secondsElapsed != 0)
    {
        switch (unitType)
        {
            case UNIT_IMPERIAL:
                displayValue("", changeToFahrenheit(getTemp()), " °F", THIRD_ROW, ALIGN_CENTRE);
                break;
            default:
                displayValue("", getTemp(), "°C", THIRD_ROW, ALIGN_CENTRE);
                break;
        }
    } else {
        displayString("Workout has not started", THIRD_ROW, ALIGN_CENTRE);
    }

    /* Fourth Line */
    displayTime("Time:", context->secondsElapsed, FOURTH_ROW, ALIGN_CENTRE);
    
}

/* State Transitions */

static void goToDistance(FitnessTrackerPtr context)
{
    changeState(context, transitionToDistance());
}