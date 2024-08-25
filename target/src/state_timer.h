#ifndef STATE_TIMER_H_
#define STATE_TIMER_H_

#include "state.h"
#include "Fitness_Tracker.h"


/* Include what it will transition into */
#include "state_stopwatch.h"
#include "state_temperature.h"
#include "state_setTimer.h"

/* Function when other change to this concrete state */
StatePtr transitionToTimer(void);

#endif