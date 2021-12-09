#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MINE_CONTROL_H_
#define MINE_CONTROL_H_

#define MINE_CONTROL_NUM_ROWS 15   
#define MINE_CONTROL_NUM_COLS 20

#define MINE_CONTROL_NUM_MINES 60

//init funciton for the mineControl. Used before all other functions.
void mineControl_init();

//normal tickfunction for the mineSweeper game top level
void mineControl_tick();

#endif /*mine_control.h*/