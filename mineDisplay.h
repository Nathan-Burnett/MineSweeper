#include <stdint.h>
#include "mineControl.h"

#ifndef MINE_DISPLAY_H_
#define MINE_DISPLAY_H_


//function to be called before all other display functions
void mineDisplay_init();

//draws a board for the game to be played
void mineDisplay_drawBoard();

// drawMines(x,y) draws and places a mine at x,y postion 
void mineDisplay_drawMines(uint8_t x, uint8_t y);

// draws the tile with its threat level or nothing or a mine. Which ever applies
void mineDisplay_revealTile(uint8_t x, uint8_t y, tile_t tile, uint8_t theatLvl);





#endif /*mine_Display.h*/