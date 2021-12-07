#include <stdint.h> 
#include "mineControl.h"

#ifndef MINE_DISPLAY_H_
#define MINE_DISPLAY_H_


//function to be called before all other display functions
void mineDisplay_init();

//draws a board for the game to be played
void mineDisplay_drawBoard();

// drawMines(x,y) draws and places a mine at x,y postion 
void mineDisplay_drawMine(uint8_t x, uint8_t y, bool redBackground);

// draws the tile with its threat level or nothing.
void mineDisplay_revealTile(uint8_t x, uint8_t y, uint8_t theatLvl);





#endif /*mine_Display.h*/