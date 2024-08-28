// *******************************************************
// 
// state.h
//
// Implementing a State Pattern 
// Ammar Ariffin
// 
// *******************************************************
#ifndef STATE_H
#define STATE_H

// Forward declaration of State structure
typedef struct State* StatePtr;

#include "Fitness_Tracker.h"

// Typedef for function *pointers
typedef void (*ActionFunction)(FitnessTrackerPtr);

// State structure
struct State {
    ActionFunction rightButPressed;
    ActionFunction leftButPressed;
    ActionFunction topButPressed;
    ActionFunction botButPressed;
    ActionFunction rightSWOn;
    ActionFunction rightSWOff;
    ActionFunction updateDisplay;
    ActionFunction botButLongPress;
    ActionFunction topButLongPress;
};

void initDefaultImplementation(StatePtr state);

#endif // STATE_H
