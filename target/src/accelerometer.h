/* 
 *   by Ammar Ariffin
 *
 * Module for getting data from
 * the accelerometer
 * 
**/

#ifndef ACCELEROMETER_H_
#define ACCELEROMETER_H_

#include "accl_hal_tiva.h"

void initAccelBuffer(void);

void pollAccelData(void);

vector3_t getAverageAccel(void);

#endif