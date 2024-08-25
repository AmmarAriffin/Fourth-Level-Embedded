/* 
 *   by Ammar Ariffin
 *
 * State for showing distance
 * 
**/

#ifndef STATE_DISTANCE_H_
#define STATE_DISTANCE_H_


#include "state.h"
#include "Fitness_Tracker.h"

/* Include what it will transition into */
#include "state_setGoal.h"

/* Function when other change to this concrete state */
StatePtr transitionToDistance(void);


#endif // DISTANCE_H_