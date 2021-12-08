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

#define PLANT   false
#define REMOVE  true
#define DRAW    false
#define ERASE   true

//states for the state machine
enum mineControl_st_t {
    init_st,
    startText_st,
    wait4FirstTouch_st,
    wait4Touch_st,
    mineExploded_st,
    winText_st,
    clearBoard_st
};
//current state
static enum mineControl_st_t currentState;

//The game is made of this field that is consisted of rows and colums of tiles that can either
//be a mine or an empty place.
typedef struct 
{
    bool isRevealed; //bool to check weather the tile has been selected and shown
    bool isMine; //bool to check weather tile is a mine
    bool isFlagged; //bool to check weather the tile is flagged^

}tile_t;
//the minefield
static tile_t mineField[MINE_CONTROL_NUM_ROWS][MINE_CONTROL_NUM_COLS];

//variable to help keep track of tiles
static uint16_t revealedTiles;
static uint16_t flaggedTiles;


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
uint8_t getThreatLvl(uint8_t x, uint8_t y) {
    //variable to count how many mines are surronding the tile
    uint8_t numMines = 0;

    //checks each row
    for (int16_t row = y - 1; row <= y + 1; ++row) {
        //checks to make sure it's not out of bounds
        if (row >= MINE_CONTROL_NUM_ROWS || row < 0) continue;
        
        //checks each entry
        for (int16_t column = x - 1; column <= x + 1; ++column) {

            //checks to make sure it's in bounds
            if (column >= MINE_CONTROL_NUM_COLS || column < 0) continue;
            
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
    uint8_t threat = getThreatLvl(x,y);

    //reveals the tile and marks it as revealed (so we don't recurse)
    mineDisplay_revealTile(x, y, threat);
    mineField[y][x].isRevealed = true;

    // if there's not a threat (lol) then reveal surrounding tiles
    if (!threat) {
        //reveal each row
        for (int16_t row = y - 1; row <= y + 1; ++row) {
            //checks to make sure it's not out of bounds
            if (row >= MINE_CONTROL_NUM_ROWS || row < 0) continue;
            
            //reveal each entry
            for (int16_t column = x - 1; column <= x + 1; ++column) {

                //checks to make sure it's in bounds
                if (column >= MINE_CONTROL_NUM_COLS || (row == y && column == x) || column < 0) continue;

                //if it's not revealed, reveal it! (scary recursion :0)
                if (!mineField[row][column].isRevealed) {
                    revealTiles(column, row);
                }
            }
        }
    }
}

//plants sets isMine to true for mines around where the user initially touches so setMines won't 
void plantFakeMines(uint8_t x, uint8_t y, bool remove) {

    //do all the columns
    for (int16_t column = x - 1; column <= x + 1; ++column) {
        //out of bounds check
        if (column >= MINE_CONTROL_NUM_COLS || column < 0) continue;
        //and all the rows
        for (int16_t row = y - 1; row <= y + 1; ++row) {
            //out of bounds check
            if (row >= MINE_CONTROL_NUM_ROWS || row < 0) continue;
            mineField[row][column].isMine = !remove;
        }
    }
    
}

void drawMines(uint8_t x, uint8_t y) {
    mineDisplay_drawMine(x,y,true);
    //draws the mines in the field
    for (ALL_COLUMNS) {
        //each entry in each column
        for (ALL_ROWS) {
            if (row == y && column == x) continue;
            //if it's a mine, draw it
            if (mineField[row][column].isMine) {
                if (!mineField[row][column].isFlagged)
                    mineDisplay_drawMine(column,row,false);
            }
            else if (mineField[row][column].isFlagged && !mineField[row][column].isRevealed) {
                mineDisplay_drawFlaggedDirt(column,row);
            }
        }
    }
}

//ADVERTISED FUNCTIONS

//init funciton for the mineControl. Used before all other functions.
void mineControl_init() {
    mineDisplay_init();
    currentState = init_st;
}

//normal tickfunction for the mineSweeper game top level
void mineControl_tick() {
    static uint16_t timer = 0;

    // switch state for transition
    switch(currentState) {
        case init_st:
            currentState = startText_st;
            mineDisplay_drawGameStart(DRAW);
            timer = 0;
            break;
        case startText_st:

            if (display_isTouched()) {
                display_clearOldTouchData();
                mineDisplay_drawGameStart(ERASE);
                //starting the game!
                currentState = wait4FirstTouch_st;
                mineDisplay_drawBoard();
                clearField();
                srand(timer);
                revealedTiles = 0;
                touchHandler_enable();
            }
            break;
        case wait4FirstTouch_st:
            //if we detect a release, it's go time. 
            if (touchHandler_releaseDetected()) {
                touchHandler_disable();
                uint8_t row, column;
                touchHandler_getTouchedRowColumn(&row, &column);

                //if there's a flag remove the flag
                if (mineField[row][column].isFlagged) {
                    mineField[row][column].isFlagged = false;
                    mineDisplay_drawFlag(column, row, REMOVE);
                }
                else {
                    plantFakeMines(column, row, PLANT);
                    setMines();
                    plantFakeMines(column, row, REMOVE);
                    revealTiles(column, row);
                    currentState = wait4Touch_st;
                }
                display_clearOldTouchData();
                touchHandler_enable();
            }
            break;
        case wait4Touch_st:
            if (touchHandler_releaseDetected()) {
                touchHandler_disable();
                int8_t row, column;
                touchHandler_getTouchedRowColumn(&row, &column);

                // if it's revealed already
                if (mineField[row][column].isRevealed) {
                    //do nothing
                }
                //if there's a flag 
                else if (mineField[row][column].isFlagged) {
                    //remove the flag
                    mineField[row][column].isFlagged = false;
                    mineDisplay_drawFlag(column, row, REMOVE);
                    touchHandler_enable();
                }
                //if there's a mine
                else if (mineField[row][column].isMine) {
                    //oops, hit a mine. 
                    currentState = mineExploded_st;
                    drawMines(column, row);
                }
                //if it's an unrevealed, unflagged, non-mine tile
                else {
                    revealTiles(column, row);
                    //if we're out of non-mine tiles
                    if (revealedTiles == NUM_TILES - MINE_CONTROL_NUM_MINES) {
                        //yo you win
                        currentState = winText_st;
                        mineDisplay_drawWinText();
                    }
                    else {
                        //stay in the same state
                        touchHandler_enable();
                    }
                }
            }
            break;
        case mineExploded_st:
            //if it's touched, move on
            if (display_isTouched()) {
                display_clearOldTouchData();
                currentState = clearBoard_st;
            }
            break;
        case winText_st:
            //if it's touched, move on
            if (display_isTouched()) {
                display_clearOldTouchData();
                currentState = clearBoard_st;
            }        
            break;
        case clearBoard_st:
            currentState = startText_st;
            mineDisplay_drawGameStart(DRAW);
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
        case wait4FirstTouch_st:
        case wait4Touch_st:
            //let's draw a flag!
            if (touchHandler_isTimedOut()) {
                int8_t row, column;
                touchHandler_getTouchedRowColumn(&row, &column);
                
                //if it's flagged
                if (mineField[row][column].isFlagged) {
                    //remove the flag
                    mineDisplay_drawFlag(column, row, REMOVE);
                    mineField[row][column].isFlagged = false;
                }
                // if it's not revealed
                else if (!mineField[row][column].isRevealed) {
                    mineDisplay_drawFlag(column, row, PLANT);
                    mineField[row][column].isFlagged = true;
                }
                //otherwise it's revealed so we don't want to do anything
            }            
            break;
        case clearBoard_st:
            mineDisplay_clearScreen();
            break;
        default:
            break;
    }
}