#include "mineDisplay.h"
#include "mineControl.h"
#include "display.h"
#include "buttons.h"
#include <stdio.h>
#include <stdint.h>

#define COL_NUM MINE_CONTROL_NUM_COLS
#define ROW_NUM MINE_CONTROL_NUM_ROWS
#define GRID_ROW_POS (DISPLAY_HEIGHT/NUM_COL)
#define GRID_COL_POS (DISPLAY_WIDTH/NUM_ROW)
#define SCREEN_EDGE 0 


//init the main display and fills it with black screen
void mineDisplay_init(){
    display_init();
    display_fillScreen(DISPLAY_BLACK);
    return;
}


//draws the minefield on the screen
void mineDisplay_drawBoard(){
    //draws colums
    for(uint8_t i = 1; i<=COL_NUM;i++){
        display_drawLine(GRID_COL_POS*i,SCREEN_EDGE,GRID_COL_POS*i,GRID_ROW_POS*ROW_NUM,DISPLAY_WHITE);
    }
    //draws rows
    for(uint8_t i = 0; i<=ROW_NUM;i++){
        display_drawLine(SCREEN_EDGE,GRID_ROW_POS*i,GRID_COL_POS*ROW_NUM,GRID_ROW_POS*i,DISPLAY_WHITE);
    }
    return;
}

void mineDisplay_drawMine(uint8_t x, uint8_t y, bool redBackground){
    return;
}


