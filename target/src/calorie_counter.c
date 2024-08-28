 
// calorie_counter.c
//
//  Created on: 28/08/2024
//  Author(s): Sabine Volker
//
//   Feature for a Fitness Monitor that is able to calculate 
//   Calories that are burned depending on someone's Weight and
//   Height and general resting metabolic rate.
// 
// *******************************************************

#include "stepCounter.h"
#include "calorie_counter.h"
#include "core.h"

//Constants
//from 
#define CALORIE_STEP_ADJUSTMENT 40 
//from https://www.healthline.com/health/fitness-exercise/how-many-calories-do-i-burn-a-day#daily-calorie-burn-by-intent
#define WEIGHT_ADJUSTMENT 999 
#define HEIGHT_ADJUSTMENT 625
#define AGE_ADJUSTMENT 492
#define DEFAULT_AGE 25
#define DEFAULT_CALORIES_BURNED 500

#define DEVISION_ADJUSTMENT_RMR 100
#define DEVISION_ADJUSTMENT_STEPS 1000
#define DEVISION_ADJUSTMENT_TIME 60000 //RMR value is per min and timer is in milliseconds. constant will allow for them to be per second.


static uint16_t WEIGHT = 60; //in kg
static uint16_t HEIGHT = 170;
static uint32_t START_TIME;

void initCalorieCounter(void)
{
    START_TIME = readCurrentTick();
}

uint16_t get_height()
{
    return HEIGHT;
}

uint16_t get_weight()
{
    return WEIGHT;
}

void increase_height()
{
    if (HEIGHT != UINT8_MAX)
    {
        HEIGHT += 1; 
    }
}

void increase_weight()
{
    if (WEIGHT != UINT8_MAX)
    {
        WEIGHT += 1; 
    }
}

void decrease_height()
{
    if (HEIGHT != 1) 
    {
        HEIGHT -= 1; 
    }

}

void decrease_weight()
{
    if (WEIGHT != 1) 
    {
        WEIGHT -= 1; 
    }
}

uint32_t calculateCalories(void)
{
    uint32_t current_time = (readCurrentTick() - START_TIME)/1000;
    uint32_t steps_taken = getStepsCount();
    uint32_t rmr = ((WEIGHT_ADJUSTMENT*WEIGHT + HEIGHT_ADJUSTMENT*HEIGHT - AGE_ADJUSTMENT * DEFAULT_AGE + DEFAULT_CALORIES_BURNED)) / (DEVISION_ADJUSTMENT_RMR);
    //rmr means resting metabolic rate. It refers to the amount of calories burned while resting
    uint32_t calories_steps = (steps_taken * CALORIE_STEP_ADJUSTMENT) / DEVISION_ADJUSTMENT_STEPS;
    uint32_t calories_burned = calories_steps + ((rmr * current_time)/60000);

    return calories_burned;
}

