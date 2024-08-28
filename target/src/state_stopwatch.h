#ifndef STATE_STOPWATCH_H_
#define STATE_STOPWATCH_H_

#include "state.h"
#include "Fitness_Tracker.h"


/* Include what it will transition into */
#include "state_distance.h"
#include "state_timer.h"

/* Function when other change to this concrete state */
StatePtr transitionToStopwatch(void);

#endif