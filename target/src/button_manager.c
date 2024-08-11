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
#define LONG_PRESS_CYCLES 20

static uint16_t longPressCount = 0;
static bool allowLongPress = true;


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
void btnUpdateState(deviceStateInfo_t* deviceStateInfo, uint16_t currentTime)
{
    updateButtons();
    updateSwitch();

    displayMode_t currentDisplayMode = deviceStateInfo ->displayMode;

    // Changing screens
    if (checkButton(LEFT) == PUSHED) {
        deviceStateInfo -> displayMode = (deviceStateInfo -> displayMode + 1) % DISPLAY_NUM_STATES;      //flicker when pressing button

    } else if (checkButton(RIGHT) == PUSHED) {
        // Can't use mod, as enums behave like an unsigned int, so (0-1)%n != n-1
        if (deviceStateInfo -> displayMode > 0) {
            deviceStateInfo -> displayMode--;
        } else {
            deviceStateInfo -> displayMode = DISPLAY_NUM_STATES-1;
        }
    }

    // Enable/Disable test mode
    if (isSwitchUp()) {
        deviceStateInfo -> debugMode = true;
    } else {
        deviceStateInfo -> debugMode = false;
    }


    // Usage of UP and DOWN buttons
    if (deviceStateInfo -> debugMode) {
        // TEST MODE OPERATION
        if (checkButton(UP) == PUSHED) {
            deviceStateInfo -> stepsTaken = deviceStateInfo -> stepsTaken + DEBUG_STEP_INCREMENT;
        }

        if (checkButton(DOWN) == PUSHED) {
            if (deviceStateInfo -> stepsTaken >= DEBUG_STEP_DECREMENT) {
                deviceStateInfo -> stepsTaken = deviceStateInfo -> stepsTaken - DEBUG_STEP_DECREMENT;
            } else {
                deviceStateInfo -> stepsTaken = 0;
            }
        }


    } else {

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
                    longPressCount++;
                    if (longPressCount >= LONG_PRESS_CYCLES) {
                        deviceStateInfo -> stepsTaken = 0;
                        flashMessage("Reset!");
                    }
                } else {
                    longPressCount = 0;
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
                    longPressCount++;
                    if (longPressCount >= LONG_PRESS_CYCLES) {
                        deviceStateInfo -> stepsTaken = 0;
                        flashMessage("Reset!");
                    }
                } else {
                    longPressCount = 0;
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

                break;
            //*****************************************************************
            case DISPLAY_STOPWATCH:
                // if ((isDown(UP) == true) && (allowLongPress)) {
                //     longPressCount++;
                //     if (longPressCount >= LONG_PRESS_CYCLES) {
                //         resetStopwatch();
                //     }
                // } else 
                if (checkButton(UP) == PUSHED) {
                    toggleStopwatch(currentTime);
                    allowLongPress = false;
                    longPressCount = 0;
                }
                if (checkButton(UP) == RELEASED) {
                    allowLongPress = true;
                }
                //#############################
                if (checkButton(DOWN) == PUSHED) {
                    // Print lap time and restart stopwatch
                }
                break;
        }

    }

}


