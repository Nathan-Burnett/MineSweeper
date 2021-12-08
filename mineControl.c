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
#include "touchHandler.h"
#include "display.h"

#define NUM_TILES   MINE_CONTROL_NUM_ROWS * MINE_CONTROL_NUM_COLS

#define ALL_ROWS    uint8_t row = 0; row < MINE_CONTROL_NUM_ROWS; ++row
#define ALL_COLUMNS uint8_t column = 0; column < MINE_CONTROL_NUM_COLS; ++column

//states for the state machine
enum mineControl_st_t {
    init_st,
    startText_st,
    wait4FirstTouch_st,
    wait4Touch_st,
    adcWait_st,
    processTouch_st,
    mineExploded_st,
    
};

//current state
static enum mineControl_st_t currentState;

//variable to help keep track of tiles
static uint16_t revealedTiles;
static uint16_t flaggedTiles;

//the minefield
static tile_t mineField[MINE_CONTROL_NUM_ROWS][MINE_CONTROL_NUM_COLS];

//HELPER FUNCTIONS

//clears the field
void clearField() {
    revealedTiles = 0;
    flaggedTiles = 0;
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
        if (!mineField[randRow][randColumn].isMine) {
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
    ++revealedTiles;
    // figures out the threat level
    uint8_t threat = mineControl_getThreatLvl(x,y);

    //reveals the tile and marks it as revealed (so we don't recurse)
    mineDisplay_revealTile(x, y, threat);
    mineField[x][y].isRevealed = true;

    // if there's not a threat (lol) then reveal surrounding tiles
    if (!threat) {
        //reveal each row
        for (int16_t row = x - 1; row <= x + 1; ++row) {
            //checks to make sure it's not out of bounds
            if (row < 0 || row >= MINE_CONTROL_NUM_ROWS) continue;
            
            //reveal each entry
            for (int16_t column = y - 1; column <= y + 1; ++column) {

                //checks to make sure it's in bounds
                if (column < 0 || column >= MINE_CONTROL_NUM_COLS || (row == y && column == x)) continue;

                //if it's not revealed, reveal it! (scary recursion :0)
                if (!mineField[row][column].isRevealed) {
                    revealTiles(column, row);
                }
            }
        }
    }
}

//reveals all mines with a red background behind the one pressed

//ADVERTISED FUNCTIONS

//init funciton for the mineControl. Used before all other functions.
void mineControl_init() {
    mineDisplay_init();
    touchHandler_init();
    currentState = init_st;
}

//normal tickfunction for the mineSweeper game top level
void mineControl_tick() {
    static uint16_t timer = 0;

    // switch state for transition
    switch(currentState) {
        case init_st:
            currentState = startText_st;
            timer = 0;
            break;
        case startText_st:

            if (display_isTouched()) {
                display_clearOldTouchData();
                currentState = wait4Touch_st;
                mineDisplay_drawBoard();
                clearField();
                setMines();
                srand(timer);
                revealedTiles = 0;
            }
            break;
        case wait4FirstTouch_st:

            if (display_isTouched()) {

            }
            break;
        case wait4Touch_st:
            break;
        case adcWait_st:
            break;
        default:
            break;
    }

    // action switch statement
    switch(currentState) {
        case init_st:
            break;
        case startText_st:
            ++timer;
            break;
        case wait4Touch_st:
            break;
        case adcWait_st:
            break;
        default:
            break;
    }
}








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

