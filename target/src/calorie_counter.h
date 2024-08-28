#ifndef CALORIE_COUNTER_H_
#define CALORIE_COUNTER_H_

// calorie_counter.h
//
//  Created on: 28/08/2024
//  Author(s): Sabine Volker
//
//   Feature for a Fitness Monitor that is able to calculate 
//   Calories that are burned depending on someone's Weight and
//   Height and general resting metabolic rate.
// 
// *******************************************************
#include <stdint.h>

void initCalorieCounter(void);

uint16_t get_height();

uint16_t get_weight();

void increase_height();

void increase_weight();

void decrease_height();

void decrease_weight();

uint32_t calculateCalories(void);





#endif /*CALORIE_COUNTER_H_*/