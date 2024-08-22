/*
 * Button_management.c
 *
 * Modifies the device's state according to the user's button and switch input
 *
 *  Created on: 31/03/2022
 *      Author: Daniel Rabbidge
 *
 *  FitnessThur9-1
 *  NOTICE: isDown returns true for UP/DOWN being pressed
 *          but false for LEFT/RIGHT. Add ! for LEFT/RIGHT
 *          as they are active low.
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/debug.h"
#include "inc/tm4c123gh6pm.h"
#include "buttons4.h"
#include "display_manager.h"
#include "button_manager.h"
#include "switches.h"
#include "stopwatch.h"
#include "timer_s.h"
#include "step_counter_main.h"
#include "utils/ustdlib.h"

//********************************************************
// Constants and static vars
//********************************************************
#define LONG_PRESS_CYCLES 30 
#define SHORTER_LONG_PRESS_CYCLES 10 // Should only be used if a button has already been
                                     // determined to have been held down

static uint16_t longPressCountUp = 0;
static uint16_t longPressCountDown = 0;
static uint16_t longPressCountLeft = 0; 
static uint16_t longPressCountRight = 0;
static bool buttonHeldLeft = false;
static bool buttonHeldRight = false;
static bool changedState = false;

//********************************************************
// Global vars
//********************************************************
uint8_t timerSelect = 0;
uint8_t placeSelect = 0;

//********************************************************
// Init buttons and switch I/O handlers
//********************************************************
void btnInit(void)
{
    initButtons();
    initSwitch();
}


//********************************************************
// Run at a fixed rate, modifies the device's state depending on button presses
//********************************************************
void btnUpdateState(deviceStateInfo_t* deviceState, uint32_t currentTime)
{
    updateButtons();
    updateSwitch();

    displayMode_t currentDisplayMode = deviceState ->displayMode;

    // Changing screens
    switch(currentDisplayMode) {
        case DISPLAY_SET_TIMER:
            if (!isDown(LEFT)) {
                checkButton(LEFT);
                longPressCountLeft++;
                if (longPressCountLeft == LONG_PRESS_CYCLES) {
                    incrementTime(timerArray[timerSelect], placeSelect);
                    buttonHeldLeft = true;
                    longPressCountLeft = 0;
                } else if (longPressCountLeft == SHORTER_LONG_PRESS_CYCLES && buttonHeldLeft){
                    //Allows faster time increments when button is held down
                    incrementTime(timerArray[timerSelect], placeSelect);
                    longPressCountLeft = 0;
                }
            } else if (checkButton(LEFT) == RELEASED && longPressCountLeft < LONG_PRESS_CYCLES) {
                incrementTime(timerArray[timerSelect], placeSelect);
                buttonHeldLeft = false;
                longPressCountLeft = 0;
            } else if (checkButton(LEFT) == NO_CHANGE ) {
                longPressCountLeft = 0;   
            }

            if (!isDown(RIGHT)) {
                checkButton(RIGHT);
                longPressCountRight++;
                if (longPressCountRight == LONG_PRESS_CYCLES) {
                    decrementTime(timerArray[timerSelect], placeSelect);
                    buttonHeldRight = true;
                    longPressCountRight = 0;
                } else if (longPressCountRight == SHORTER_LONG_PRESS_CYCLES && buttonHeldRight){
                    //Allows faster time decrements when button is held down
                    decrementTime(timerArray[timerSelect], placeSelect);
                    longPressCountRight = 0;
                }
            } else if (checkButton(RIGHT) == RELEASED && longPressCountRight < LONG_PRESS_CYCLES) {
                decrementTime(timerArray[timerSelect], placeSelect);
                buttonHeldRight = false;
                longPressCountRight = 0;
            } else if (checkButton(RIGHT) == NO_CHANGE ) {
                longPressCountRight = 0;   
            }
        //*****************************************************************
        default:
            if (checkButton(LEFT) == PUSHED) {
            deviceState->displayMode = (deviceState->displayMode+1) % (DISPLAY_NUM_STATES - SET_DISPLAY_NUM);    
            } else if (checkButton(RIGHT) == PUSHED) {
                // Can't use mod, as enums behave like an unsigned int, so (0-1)%n != n-1
                if (deviceState->displayMode > 0) {
                    deviceState->displayMode--;
                } else {
                    deviceState->displayMode = DISPLAY_NUM_STATES-1-SET_DISPLAY_NUM;
                }
            };
            break;
    }
    
    // Usage of UP and DOWN buttons

    switch(currentDisplayMode) {
        //*****************************************************************
        case DISPLAY_STEPS:
            if (checkButton(UP) == PUSHED) {
                if (deviceState->displayUnits == UNITS_SI) {
                    deviceState->displayUnits = UNITS_ALTERNATE;
                } 
                else {
                    deviceState->displayUnits = UNITS_SI;
                }
            }
            //#############################
            if (isDown(DOWN) == true) {
                longPressCountDown++;
                if (longPressCountDown >= LONG_PRESS_CYCLES) {
                    deviceState->stepsTaken = 0;
                    flashMessage("Reset!");
                }
            } else {
                longPressCountDown = 0;
            }
            break;
        //*****************************************************************
        case DISPLAY_DISTANCE:
            if (checkButton(UP) == PUSHED) {
                if (deviceState->displayUnits == UNITS_SI) {
                    deviceState->displayUnits = UNITS_ALTERNATE;
                } 
                else {
                    deviceState->displayUnits = UNITS_SI;
                }
            }
            //#############################
            if (isDown(DOWN) == true) {
                longPressCountDown++;
                if (longPressCountDown >= LONG_PRESS_CYCLES) {
                    deviceState->stepsTaken = 0;
                    flashMessage("Reset!");
                }
            } else {
                longPressCountDown = 0;
            }
            break;
        //*****************************************************************
        case DISPLAY_SET_GOAL:
            if (checkButton(UP) == PUSHED) {
                if (deviceState->displayUnits == UNITS_SI) {
                    deviceState->displayUnits = UNITS_ALTERNATE;
                } 
                else {
                    deviceState->displayUnits = UNITS_SI;
                }
            }
            //#############################
            if (checkButton(DOWN) == PUSHED) {
                deviceState->currentGoal = deviceState->newGoal;
                char toDraw[DISPLAY_WIDTH+1];
                usnprintf(toDraw, DISPLAY_WIDTH + 1, "GOAL SET: %d", deviceState->newGoal);
                flashMessage(toDraw);
                deviceState->displayMode = DISPLAY_STEPS;
            }
            break;
        //*****************************************************************
        case DISPLAY_TIMER:
            if (isDown(UP)) {
                checkButton(UP);
                longPressCountUp++;
                if (longPressCountUp >= LONG_PRESS_CYCLES) {
                    resetTimer(timerArray[timerSelect]);
                } 
            } else if (checkButton(UP) == RELEASED && longPressCountUp < LONG_PRESS_CYCLES) {
                toggleTimer(timerArray[timerSelect]);
                longPressCountUp = 0;
            } else if (checkButton(UP) == NO_CHANGE ) {
                longPressCountUp = 0;   
            }
            //#############################
            if (isDown(DOWN)) {
                checkButton(DOWN);
                longPressCountDown++;
                if (longPressCountDown >= LONG_PRESS_CYCLES && !changedState) {
                    deviceState->displayMode = DISPLAY_SET_TIMER;
                    changedState = true;
                } 
            } else if (checkButton(DOWN) == RELEASED && longPressCountDown < LONG_PRESS_CYCLES) {
                timerSelect = (timerSelect + 1) % NUM_TIMERS; 
                longPressCountDown = 0;
            } else if (checkButton(DOWN) == NO_CHANGE ) {
                changedState = false;
                longPressCountDown = 0;   
            }
            break;
        //*****************************************************************
        case DISPLAY_SET_TIMER:
            if (checkButton(UP) == PUSHED) {
                placeSelect = (placeSelect + 1) % 3;
            }
            //#############################
            if (isDown(DOWN)) {
                checkButton(DOWN);
                longPressCountDown++;
                if (longPressCountDown >= LONG_PRESS_CYCLES && !changedState) {
                    deviceState->displayMode = DISPLAY_TIMER;
                    changedState = true;
                } 
            } else if (checkButton(DOWN) == RELEASED && longPressCountDown < LONG_PRESS_CYCLES) {
                //Change viewed timer
                timerSelect = (timerSelect + 1) % NUM_TIMERS;
                longPressCountDown = 0;
            } else if (checkButton(DOWN) == NO_CHANGE ) {
                changedState = false;
                longPressCountDown = 0;   
            }
            break;
        //*****************************************************************
        case DISPLAY_STOPWATCH:
            if (isDown(UP)) {
                checkButton(UP);
                longPressCountUp++;
                if (longPressCountUp >= LONG_PRESS_CYCLES) {
                    resetStopwatch();
                } 
            } else if (checkButton(UP) == RELEASED && longPressCountUp < LONG_PRESS_CYCLES) {
                toggleStopwatch();
                longPressCountUp = 0;
            } else if (checkButton(UP) == NO_CHANGE ) {
                longPressCountUp = 0;   
            }
            //#############################
            if (checkButton(DOWN) == PUSHED) {
                storeLap();
            }
            break;
        //*****************************************************************
        default:
            //Displays error if this case is reached
            deviceState->displayMode = DISPLAY_NUM_STATES;
        break;
    }
}

//Example code for long button presses
// if (isDown(DOWN)) { // NOTICE: isDown returns true for UP/DOWN being pressed
//                                but false for LEFT/RIGHT. Add ! for LEFT/RIGHT
//                                as they are active low.
//                 checkButton(DOWN);
//                 longPressCountDown++;
//                 if (longPressCountDown >= LONG_PRESS_CYCLES) {
//                     // DO SOMETHING ON LONG PRESS
//                     changedState = true;
//                 } 
//             } else if (checkButton(DOWN) == RELEASED && longPressCountDown < LONG_PRESS_CYCLES) {
//                 // DO SOMETHING ON SHORT PRESS
//                 longPressCountDown = 0;
//             } else if (checkButton(DOWN) == NO_CHANGE ) {
//                 longPressCountDown = 0;   
//             }