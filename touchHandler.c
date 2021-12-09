/**
 * @file touchHandler.c
 * @author Topher Johnson
 * @version 0.1
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "touchHandler.h"
#include <stdbool.h>
#include <stdint.h>
#include "display.h"
#include "mineControl.h"

#define ROW_HEIGHT  (DISPLAY_HEIGHT / MINE_CONTROL_NUM_ROWS)
#define COL_WIDTH   (DISPLAY_WIDTH / MINE_CONTROL_NUM_COLS)

#define TOUCH_TIMER_MAX     4

// states for buttonhandler
enum touchHandler_st_t {
  init_st,             // waiting for enable()
  waitingForTouch_st,  // waiting for touch
  adcTimerRunning_st,  // waiting for adc to settle
  currentlyTouched_st, // waiting for release
  timerExpired_st,     // waiting for release
  setFlag_st,
  final_st             // waiting for disable()
};

static enum touchHandler_st_t currentState;
static bool enable;
static bool released;
static uint8_t touchedRow, touchedColumn;

//gets the touched region row and column 
void touchHandler_getTouchedRowColumn(uint8_t *row, uint8_t *column) {
    *row = touchedRow;
    *column = touchedColumn;
}

//turns on the state machine
void touchHandler_enable() {
    enable = true;
}

//turns off the state machine
void touchHandler_disable() {
    enable = false;
    released = false;
    currentState = init_st;
}

//waits to see if there's a release
bool touchHandler_releaseDetected() { return released; }

//lets us know the touchhandler is over
bool touchHandler_isTimedOut() { 
    if (currentState == setFlag_st) {
        currentState = timerExpired_st;
        return true;
    }
    else return false;
}

//inits the SM
void touchHandler_init() {
    enable = false;
    released = false;
    currentState = init_st;
}

//tick function
void touchHandler_tick() {
    static uint8_t timer = 0;
    // Perform state update first.
    switch (currentState) {
    case init_st:
        // if it's enabled, wait for a touch
        if (enable) {
            currentState = waitingForTouch_st;
        }
        break;
    case waitingForTouch_st:
        // if it's touched, move to the adc timer state
        if (display_isTouched()) {
            display_clearOldTouchData();
            currentState = adcTimerRunning_st;
        }
        break;
    case adcTimerRunning_st:
        // the period is 100ms, so we don't really need a timer.
        // We'll use a mealy action here else the getTouchData could
        // potentially happen right after the press
        {
            int16_t x, y;
            uint8_t z;
            // gets the touch data from the display
            display_getTouchedPoint(&x, &y, &z);
            touchedRow = y / ROW_HEIGHT;
            touchedColumn = x / COL_WIDTH;
        }
        currentState = currentlyTouched_st;
        timer = 0;
        break;
    case currentlyTouched_st:
        // if the timer runs out, then move on
        if (timer == TOUCH_TIMER_MAX) {
            currentState = setFlag_st;
        }
        // if the display isn't touched, change released to true
        if (!display_isTouched()) {
            released = true;
            currentState = final_st;
        }
        break;
    case setFlag_st:
        //do nothing. wait for this to be changed in isTimedOut
        break;
    case timerExpired_st:
        //wait for a release before moving to final_st
        if (!display_isTouched()) {
            display_clearOldTouchData();
            currentState = init_st;
        }
    case final_st:
        // This won't be called ever
        if (!enable) {
            currentState = init_st;
        }
        break;
    default:
        break;
    }

    // Performs state action next
    switch (currentState) {
    case currentlyTouched_st:
        ++timer;
        break;
    default:
        break;
    }
}
