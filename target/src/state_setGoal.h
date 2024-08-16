#ifndef STATE_SETGOAL_H_
#define STATE_SETGOAL_H_

#include "state.h"
#include "Fitness_Tracker.h"


/* Include what it will transition into */
#include "state_temperature.h"

/* Function when other change to this concrete state */
StatePtr transitionToSetGoal(void);


#endif