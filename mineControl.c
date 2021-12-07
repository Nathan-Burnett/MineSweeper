/**
 * @file mineControl.c
 * @author Topher Johnson
 * @version 0.1
 * @date 2021-12-06
 * 
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mineControl.h"
#include "mineDisplay.h"
#include "display.h"

#define NUM_TILES   MINE_CONTROL_NUM_ROWS * MINE_CONTROL_NUM_COLS

#define ALL_ROWS    uint8_t row = 0; row < MINE_CONTROL_NUM_ROWS; ++row
#define ALL_COLUMNS uint8_t column = 0; column < MINE_CONTROL_NUM_COLS; ++column

//states for the state machine
enum mineControl_st_t {
    init_st,
    startText_st,
    wait4Touch_st,
    adcWait_st,

};

static enum mineControl_st_t mineControl_st;

//the minefield
static tile_t mineField[MINE_CONTROL_NUM_ROWS][MINE_CONTROL_NUM_COLS];

//HELPER FUNCTIONS

//clears the field
void clearField() {
    //for each row
    for (ALL_ROWS) {
        //for each entry in each row
        for (ALL_COLUMNS) {
            mineField[row][column].isRevealed = false;
            mineField[row][column].isMine = false;
            mineField[row][column].isFlagged = false;
        }
    }
}

//sets mines randomly
void setMines() {
    //sets a specific number of tiles to mines
    for (uint8_t i = 0; i < MINE_CONTROL_NUM_MINES; ++i) {

        uint8_t randColumn = rand() % MINE_CONTROL_NUM_COLS;
        uint8_t randRow = rand() % MINE_CONTROL_NUM_ROWS;

        //tests to see if there's a mine already
        if (mineField[randRow][randColumn].isMine) {
            mineField[randRow][randColumn].isMine = true;
        }
        //if there is, add another iteration
        else {
            --i;
        }
    }

}

// getThreatLvl(x,y) takes a x and y postion on the field and checks the threat level of that tile
uint8_t mineControl_getThreatLvl(uint8_t x, uint8_t y) {
    //variable to count how many mines are surronding the tile
    uint8_t numMines = 0;

    //checks each row
    for (int16_t row = x - 1; row <= x + 1; ++row) {
        //checks to make sure it's not out of bounds
        if (row < 0 || row >= MINE_CONTROL_NUM_ROWS) continue;
        
        //checks each entry
        for (int16_t column = y - 1; column <= y + 1; ++column) {

            //checks to make sure it's in bounds
            if (column < 0 || column >= MINE_CONTROL_NUM_COLS) continue;
            
            //if there's a mine, increase the mine count
            if (mineField[row][column].isMine) ++numMines;
        }
    }

    return numMines;
}

//recursively reveals tiles
void revealTiles(uint8_t x, uint8_t y) {
    uint8_t threat = mineControl_getThreatLvl(x,y);

    mineDisplay_revealTile(x, y, threat);
}

//reveals all mines with a red background behind the one pressed

//ADVERTISED FUNCTIONS

//init funciton for the mineControl. Used before all other functions.
void mineControl_init() {
    mineDisplay_init();
    mineControl_st = init_st;
}

//normal tickfunction for the mineSweeper game top level
void mineControl_tick();








//PROBABLY UNNEEDED FUNCTIONS

//isMine(x,y,tile) takes an x/y postion on the field and returns a bool indicating weather the tile is a mine or not
bool mineControl_isMine(uint8_t x, uint8_t y) {
    return mineField[x][y].isMine;
}

//sets a specific tile to be a flag
void mineControl_setFlag(uint8_t x, uint8_t y) {
    mineField[x][y].isFlagged = true;
}

//isRevealed(x,y,tile) takes an x and y postion on the field and returns a bool indicating weather the tile has already be selected
bool mineControl_isRevealed(uint8_t x, uint8_t y) {
    return mineField[x][y].isRevealed;
}

