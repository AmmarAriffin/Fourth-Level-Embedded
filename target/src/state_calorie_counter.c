

// Inspiration from here
// https://www.adamtornhill.com/Patterns%20in%20C%202,%20STATE.pdf
// Ammar Ariffin and Sabine Volker

#include "state.h"
#include "Fitness_Tracker.h"
#include "displayInterface.h"
#include "calorie_counter.h"


/* Include what it will transition into */
#include "state_timer.h"
#include "state_setGoal.h"

/* Generic Functions to default to when you dont declare on other*/
static void RightButPressed(FitnessTrackerPtr context)
{
    changeState(context, transitionToSetGoal());
}

static void LeftButPressed(FitnessTrackerPtr context)
{
    changeState(context, transitionToTemperature());

}

static void TopButPressed(FitnessTrackerPtr context)
{
    increase_height();
}

static void BotButPressed(FitnessTrackerPtr context)
{
    decrease_height();
}

static void RightSWOn(FitnessTrackerPtr context)
{
    
}

static void RightSWOff(FitnessTrackerPtr context)
{
    
}

static void updateDisplay(FitnessTrackerPtr context)
{
    uint16_t height = get_height();
    uint16_t weight = get_weight();
    uint32_t current_calories = calculateCalories();
    
    displayString("Calories Burned", FIRST_ROW, ALIGN_CENTRE);
    displayValue("Height: ", height,"cm",SECOND_ROW, ALIGN_CENTRE, false);
    displayValue("Weight: ", weight,"kg",THIRD_ROW, ALIGN_CENTRE, false);
    displayValue("", current_calories, "cal", FOURTH_ROW, ALIGN_CENTRE, false);

}

static void topButLongPress(FitnessTrackerPtr context)
{
    increase_weight();
}

static void botButLongPress(FitnessTrackerPtr context)
{
    decrease_weight();
}


StatePtr transitionToCalorieCounter(void)
{
    static struct State startedState;
    static int initialised = 0;
    if (0 == initialised)
    {
        initDefaultImplementation(&startedState);
        initCalorieCounter();
        
        startedState.leftButPressed = LeftButPressed;
        startedState.rightButPressed = RightButPressed;
        startedState.topButPressed = TopButPressed;
        startedState.botButPressed = BotButPressed;
        startedState.rightSWOn = RightSWOn;
        startedState.rightSWOff = RightSWOff;
        startedState.updateDisplay = updateDisplay;
        startedState.botButLongPress = botButLongPress;
        startedState.topButLongPress = topButLongPress;
        initialised = 1;
    }
    return (&startedState);

}


