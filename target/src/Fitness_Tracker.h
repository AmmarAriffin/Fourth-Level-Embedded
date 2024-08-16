#ifndef Fitness_Tracker_H_
#define Fitness_Tracker_H_

#include "state.h"


typedef struct FitnessTracker* FitnessTrackerPtr;

/* Main Context */
// For delegating behaviour to its current state

struct FitnessTracker
{
    StatePtr state;
    uint32_t secondsElapsed;
};

void changeState(FitnessTrackerPtr instance, StatePtr newState);

void startTracker(FitnessTrackerPtr instance);

void pollGPIO(FitnessTrackerPtr instance);

void display(FitnessTrackerPtr instance);



/* Dont need to really use this in main.c*/
void rightButPressed(FitnessTrackerPtr instance);

void leftButPressed(FitnessTrackerPtr instance);

void topButPressed(FitnessTrackerPtr instance);

void botButPressed(FitnessTrackerPtr instance);

void leftSwitchON(FitnessTrackerPtr instance);

void botButLongPress(FitnessTrackerPtr instance);


#endif /* Fitness_Tracker_H */