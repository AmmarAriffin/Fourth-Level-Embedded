/*
 * Button_management.c
 *
 * Modifies the device's state according to the user's button and switch input
 *
 *  Created on: 31/03/2022
 *      Author: Daniel Rabbidge
 *
 *  FitnessThur9-1
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
#include "stopwatch_timer.h"

//********************************************************
// Constants and static vars
//********************************************************
#define LONG_PRESS_CYCLES 30

static uint16_t longPressCountUp = 0;
static uint16_t longPressCountDown = 0;
static bool changedState = false;

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
void btnUpdateState(deviceStateInfo_t* deviceStateInfo, uint32_t currentTime)
{
    updateButtons();
    updateSwitch();

    displayMode_t currentDisplayMode = deviceStateInfo ->displayMode;

    // Changing screens
    if (deviceStateInfo->displayMode < (DISPLAY_NUM_STATES - SET_DISPLAY_NUM)) {
        if (checkButton(LEFT) == PUSHED) {
        deviceStateInfo -> displayMode = (deviceStateInfo -> displayMode+1) % (DISPLAY_NUM_STATES - SET_DISPLAY_NUM);    
        } else if (checkButton(RIGHT) == PUSHED) {
            // Can't use mod, as enums behave like an unsigned int, so (0-1)%n != n-1
            if (deviceStateInfo -> displayMode > 0) {
                deviceStateInfo -> displayMode--;
            } else {
                deviceStateInfo -> displayMode = DISPLAY_NUM_STATES-1-SET_DISPLAY_NUM;
            }
        }
    } else {
        switch(currentDisplayMode) {
            case DISPLAY_SET_TIMER:
                if (checkButton(LEFT)) {
                    // Increment timer digit value
                } else if (checkButton(RIGHT)) {
                    // Decrement timer digit value
                }
                break;
            //*****************************************************************
            default:
                //Displays error if this case is reached
                deviceStateInfo->displayMode = DISPLAY_NUM_STATES;
            break;
        }
    }
    

    // Usage of UP and DOWN buttons

    switch(currentDisplayMode) {
        //*****************************************************************
        case DISPLAY_STEPS:
            if (checkButton(UP) == PUSHED) {
                if (deviceStateInfo -> displayUnits == UNITS_SI) {
                    deviceStateInfo -> displayUnits = UNITS_ALTERNATE;
                } 
                else {
                    deviceStateInfo -> displayUnits = UNITS_SI;
                }
            }
            //#############################
            if (isDown(DOWN) == true) {
                longPressCountDown++;
                if (longPressCountDown >= LONG_PRESS_CYCLES) {
                    deviceStateInfo -> stepsTaken = 0;
                    flashMessage("Reset!");
                }
            } else {
                longPressCountDown = 0;
            }
            break;
        //*****************************************************************
        case DISPLAY_DISTANCE:
            if (checkButton(UP) == PUSHED) {
                if (deviceStateInfo -> displayUnits == UNITS_SI) {
                    deviceStateInfo -> displayUnits = UNITS_ALTERNATE;
                } 
                else {
                    deviceStateInfo -> displayUnits = UNITS_SI;
                }
            }
            //#############################
            if (isDown(DOWN) == true) {
                longPressCountDown++;
                if (longPressCountDown >= LONG_PRESS_CYCLES) {
                    deviceStateInfo -> stepsTaken = 0;
                    flashMessage("Reset!");
                }
            } else {
                longPressCountDown = 0;
            }
            break;
        //*****************************************************************
        case DISPLAY_SET_GOAL:
            if (checkButton(UP) == PUSHED) {
                if (deviceStateInfo -> displayUnits == UNITS_SI) {
                    deviceStateInfo -> displayUnits = UNITS_ALTERNATE;
                } 
                else {
                    deviceStateInfo -> displayUnits = UNITS_SI;
                }
            }
            //#############################
            if (checkButton(DOWN) == PUSHED) {
                deviceStateInfo -> currentGoal = deviceStateInfo -> newGoal;
                deviceStateInfo -> displayMode = DISPLAY_STEPS;
            }
            break;
        //*****************************************************************
        case DISPLAY_TIMER:
            if (isDown(UP)) {
                checkButton(UP);
                longPressCountUp++;
                if (longPressCountUp >= LONG_PRESS_CYCLES) {
                    //Reset timer to saved time
                } 
            } else if (checkButton(UP) == RELEASED && longPressCountUp < LONG_PRESS_CYCLES) {
                //Start or pause the timer
                longPressCountUp = 0;
            } else if (checkButton(UP) == NO_CHANGE ) {
                longPressCountUp = 0;   
            }
            //#############################
            if (isDown(DOWN)) {
                checkButton(DOWN);
                longPressCountDown++;
                if (longPressCountDown >= LONG_PRESS_CYCLES && !changedState) {
                    deviceStateInfo->displayMode = DISPLAY_SET_TIMER;
                    changedState = true;
                } 
            } else if (checkButton(DOWN) == RELEASED && longPressCountDown < LONG_PRESS_CYCLES) {
                //Change viewed timer
                longPressCountDown = 0;
            } else if (checkButton(DOWN) == NO_CHANGE ) {
                changedState = false;
                longPressCountDown = 0;   
            }
            break;
        //*****************************************************************
        case DISPLAY_SET_TIMER:
            if (checkButton(UP) == PUSHED) {
                //Change timer digit place
            }
            //#############################
            if (isDown(DOWN)) {
                checkButton(DOWN);
                longPressCountDown++;
                if (longPressCountDown >= LONG_PRESS_CYCLES && !changedState) {
                    deviceStateInfo->displayMode = DISPLAY_TIMER;
                    changedState = true;
                } 
            } else if (checkButton(DOWN) == RELEASED && longPressCountDown < LONG_PRESS_CYCLES) {
                //Change viewed timer
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
                toggleStopwatch(currentTime);
                longPressCountUp = 0;
            } else if (checkButton(UP) == NO_CHANGE ) {
                longPressCountUp = 0;   
            }
            //#############################
            if (checkButton(DOWN) == PUSHED) {
                storeLap(currentTime);
            }
            break;
        //*****************************************************************
        default:
            //Displays error if this case is reached
            deviceStateInfo->displayMode = DISPLAY_NUM_STATES;
        break;
    }
}


