/*
 * display_manager.h
 *
 *  Created on: 10/04/2022
 *      Author: Matthew Suter, Tim Preston-Marshall, Daniel Rabbidge
 *
 *  FitnessThur9-1
 */

#ifndef STEP_COUNTER_MAIN_H_
#define STEP_COUNTER_MAIN_H_

#include "timer_s.h"

#define M_PER_STEP 9/10
#define MAX_STR_LEN 16

#define DEBUG_STEP_INCREMENT 100
#define DEBUG_STEP_DECREMENT 500


#define SET_DISPLAY_NUM 1 // Number of display states which shouldn't be accessible
                          // through left and right buttons

#define NUM_TIMERS 4
#define TICK_MODIFIER 10 //Changes ticks to milliseconds

typedef enum {
    DISPLAY_STEPS = 0,
    DISPLAY_DISTANCE,
    DISPLAY_SET_GOAL,
    DISPLAY_TEMPERATURE,
    DISPLAY_TIMER,
    DISPLAY_STOPWATCH,
    DISPLAY_SET_TIMER,
    DISPLAY_NUM_STATES, // Automatically enumerates to the number of display states there can be
} displayMode_t;


typedef enum {
    UNITS_SI = 0,       // Steps  /km
    UNITS_ALTERNATE,    // Percent/miles
    UNITS_NUM_TYPES,
} displayUnits_t;


typedef struct {
    uint32_t stepsTaken;
    uint32_t currentGoal;       // Goal the user is aiming for
    uint32_t newGoal;           // Value from the potentiometer, processed to give a new goal
    uint32_t currentTemp;
    bool debugMode;             // Is enable/disable debug functionality
    displayMode_t displayMode;
    displayUnits_t displayUnits;
    unsigned long workoutStartTick;

    // For displaying temporary messages over the top of everything else
    char *flashMessage;
    unsigned long flashTicksLeft;
} deviceStateInfo_t;


extern timer_s timerArray[NUM_TIMERS]; // Declare timers
unsigned long readCurrentTick(void);
void flashMessage(char* toShow);

#endif /* STEP_COUNTER_MAIN_H_ */
