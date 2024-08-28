

// Inspiration from here
// https://www.adamtornhill.com/Patterns%20in%20C%202,%20STATE.pdf
// Ammar Ariffin

#include "state.h"
#include "Fitness_Tracker.h"

/* Generic Functions to default to when you dont declare on other*/
static void defaultRightButPressed(FitnessTrackerPtr instance)
{

}

static void defaultLeftButPressed(FitnessTrackerPtr instance)
{
    
}

static void defaultTopButPressed(FitnessTrackerPtr instance)
{
    
}

static void defaultBotButPressed(FitnessTrackerPtr instance)
{
}

static void defaultLeftSWPressed(FitnessTrackerPtr instance)
{
    
}

static void defaultupdateDisplay(FitnessTrackerPtr instance)
{

}

static void defaultbotButLongPress(FitnessTrackerPtr instance)
{
}

static void defaulttopButLongPress(FitnessTrackerPtr instance)
{

}

void initDefaultImplementation(StatePtr state)
{
    state->leftButPressed = defaultLeftButPressed;
    state->rightButPressed = defaultRightButPressed;
    state->topButPressed = defaultTopButPressed;
    state->botButPressed = defaultBotButPressed;
    state->leftSWPressed = defaultLeftSWPressed;
    state->updateDisplay = defaultupdateDisplay;
    state->botButLongPress = defaultbotButLongPress;
    state->topButLongPress = defaulttopButLongPress;
}


