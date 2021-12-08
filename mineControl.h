#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MINE_CONTROL_H_
#define MINE_CONTROL_H_

#define MINE_CONTROL_NUM_ROWS 10
#define MINE_CONTROL_NUM_COLS 10

#define MINE_CONTROL_NUM_MINES 10


//The game is made of this field that is consisted of rows and colums of tiles that can either
//be a mine or an empty place.
typedef struct 
{
    bool isRevealed; //bool to check weather the tile has been selected and shown
    bool isMine; //bool to check weather tile is a mine
    bool isFlagged; //bool to check weather the tile is flagged^

}tile_t;

//init funciton for the mineControl. Used before all other functions.
void mineControl_init();

//normal tickfunction for the mineSweeper game top level
void mineControl_tick();

// getThreatLvl(x,y) takes a x and y postion on the field and checks the threat level of that tile
uint8_t mineControl_getThreatLvl(uint8_t x, uint8_t y);

//isRevealed(x,y,tile) takes an x and y postion on the field and returns a bool indicating weather the tile has already be selected
bool mineControl_isRevealed(uint8_t x, uint8_t y);

//isMine(x,y,tile) takes an x/y postion on the field and returns a bool indicating weather the tile is a mine or not
bool mineControl_isMine(uint8_t x, uint8_t y);

//sets a specific tile to be a flag
void mineControl_setFlag(uint8_t x, uint8_t y);

void setMines();

#endif /*mine_control.h*/