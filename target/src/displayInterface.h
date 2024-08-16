/*
 * displayInterface.h
 *
 * Interface to the OLED used in the Tiva Board
 *
 *  Created on: 15/08/24
 *      Author: Ammar Ariffin
 *
 *  Fitness Tracker
 */

#ifndef DISPLAYINTERFACE_H_
#define DISPLAYINTERFACE_H_

#include <stdint.h>

typedef enum {
    ALIGN_LEFT = 0,
    ALIGN_CENTRE,
    ALIGN_RIGHT,
} textAlignment_t;


typedef enum {
    FIRST_ROW = 0,
    SECOND_ROW,
    THIRD_ROW,
    FOURTH_ROW,
} rowNum_t;


void displayInit(void); // Initalize the display

void displayValue(char* prefix, int32_t intValue, char*suffix, uint8_t row, textAlignment_t alignment);

void displayString(char* inStr, uint8_t row, textAlignment_t alignment);

void displayTime(char* prefix, uint16_t time, uint8_t row, textAlignment_t alignment);

#endif /* DISPLAYINTERFACE_H_ */
