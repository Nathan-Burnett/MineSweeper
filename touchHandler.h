#ifndef TOUCH_HANDLER_H_
#define TOUCH_HANDLER_H_

#include <stdint.h>

//gets the touched region row and column 
void touchHandler_getTouchedRowColumn(uint8_t *row, uint8_t *column);

//turns on the state machine
void touchHandler_enable();

//turns off the state machine
void touchHandler_disable();

//waits to see if there's a release
bool touchHandler_releaseDetected();

//lets us know the touchhandler is over
bool touchHandler_isComplete();

//inits the SM
void touchHandler_init();

//tick function
void touchHandler_tick();


#endif //TOUCH_HANDLER_H_