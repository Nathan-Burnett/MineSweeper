/**
 * @file mineControl.c
 * @author Topher Johnson
 * @version 0.1
 * @date 2021-12-06
 * 
 */


#include <stdio.h>
#include <stdint.h>
#include "mineControl.h"

//the minefield
static tile_t mineField[MINE_CONTROL_NUM_COLS][MINE_CONTROL_NUM_ROWS];

//init funciton for the mineControl. Used before all other functions.
void mineControl_init();

//normal tickfunction for the mineSweeper game top level
void mineControl_tick();

// getThreatLvl(x,y) takes a x and y postion on the field and checks the threat level of that tile
uint8_t mineControl_getThreatLvl(uint8_t x, uint8_t y);

//isRevealed(x,y,tile) takes an x and y postion on the field and returns a bool indicating weather the tile has already be selected
bool mineControl_isRevealed(uint8_t x, uint8_t y,tile_t tile);

//isMine(x,y,tile) takes an x/y postion on the field and returns a bool indicating weather the tile is a mine or not
bool mineControl_isMine(uint8_t x, uint8_t y, tile_t tile);

//sets a specific tile to be a flag
void mineControl_setFlag(tile_t tile);