#ifndef Fitness_Tracker_H_
#define Fitness_Tracker_H_

#include "state.h"
#include <stdint.h>
#include <stdbool.h>

typedef struct FitnessTracker* FitnessTrackerPtr;

/* Main Context */
// For delegating behaviour to its current state

struct FitnessTracker
{
    StatePtr state;
    unsigned long timeElapsed;
    unsigned long workoutStartTick;
};

void changeState(FitnessTrackerPtr instance, StatePtr newState);

void startTracker(FitnessTrackerPtr instance);

void pollGPIO(FitnessTrackerPtr instance);

void display(FitnessTrackerPtr instance);



/* Dont need to really use this in main.c*/
void rightButPressed(FitnessTrackerPtr instance);

void leftButPressed(FitnessTrackerPtr instance);

void topButHandler(FitnessTrackerPtr instance);

void botButHandler(FitnessTrackerPtr instance);

void leftSwitchON(FitnessTrackerPtr instance);



#endif /* Fitness_Tracker_H */