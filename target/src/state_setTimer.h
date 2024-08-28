#ifndef STATE_SETTIMER_H_
#define STATE_SETTIMER_H_

#include "state.h"
#include "Fitness_Tracker.h"


/* Include what it will transition into */
#include "state_timer.h"

/* Function when other change to this concrete state */
StatePtr transitionToSetTimer(void);

#endif