#ifndef STATE_TEMPERATURE_H_
#define STATE_TEMPERATURE_H_

#include "state.h"
#include "Fitness_Tracker.h"


/* Include what it will transition into */
#include "state_distance.h"

/* Function when other change to this concrete state */
StatePtr transitionToTemperature(void);

#endif