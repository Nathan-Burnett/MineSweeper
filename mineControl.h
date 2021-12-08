#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MINE_CONTROL_H_
#define MINE_CONTROL_H_

#define MINE_CONTROL_NUM_ROWS 12
#define MINE_CONTROL_NUM_COLS 16

#define MINE_CONTROL_NUM_MINES 30

//init funciton for the mineControl. Used before all other functions.
void mineControl_init();

//normal tickfunction for the mineSweeper game top level
void mineControl_tick();

#endif /*mine_control.h*/