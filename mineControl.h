#include <stdio.h>
#include <stdint.h>

#ifndef MINE_CONTROL_H_
#define MINE_CONTROL_H_

#define MINE_CONTROL_NUM_ROWS 10
#define MINE_CONTROL_NUM_COLS 10


//The game is made of this field that is consisted of rows and colums of tiles that can either
//be a mine or an empty place.
typedef struct 
{
    bool isRevealed; //bool to check weather the tile has been selected and shown
    bool isMine; //bool to check weather tile is a mine
    bool isFlagged; //bool to check weather the tile is flagged^

}tile_t;

//This is the selected tile that a player will choose whice is defined by a row and coloum.
typedef struct {
    uint8_t row;
    uint8_t col;
}selected_tile;

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



#endif /*mine_control.h*/