#include "mineDisplay.h"
#include "mineControl.h"
#include "display.h"
#include "buttons.h"
#include <stdio.h>
#include <stdint.h>

//text sizes for the various needs
#define TEXT_SIZE 2
#define INSTUCTION_SIZE 1
#define TITLE_TXT_SIZE 3
//title define values
#define TITLE_CURSOR_X 65
#define TITLE_CURSOR_Y 80
#define TITLE_MSG "MINESWEEPER"
//Start message define values
#define START_CURSOR_X 78
#define START_CURSOR_Y 160
#define START_MSG "Press to Start"
//flag and press message define values
#define FLAG_MSG "Long press to place and remove flag"
#define PRESS_MSG "Tap tile to uncover safe tiles"
#define FLAG_CURSOR_X 60
#define FLAG_CURSOR_Y 120
#define PRESS_CURSOR_X 70
#define PRESS_CURSOR_Y 130


#define COL_NUM MINE_CONTROL_NUM_COLS
#define ROW_NUM MINE_CONTROL_NUM_ROWS
#define GRID_ROW_POS (DISPLAY_HEIGHT/NUM_ROW)
#define GRID_COL_POS (DISPLAY_WIDTH/NUM_COL
#define CHAR_OFFSET_X ((GRID_COL_POS-(DISPLAY_CHAR_WIDTH-1)*TEXT_SIZE)/2)
#define CHAR_OFFSET_Y ((GRID_ROW_POS-DISPLAY_CHAR_WIDTH*TEXT_SIZE)/2)
#define SCREEN_EDGE 0

#define FLAG_CHAR 'F'
#define MINE_CHAR 'M'



//init the main display and fills it with black screen
void mineDisplay_init(){
    display_init();
    display_fillScreen(DISPLAY_BLACK);
    return;
}


//draws the minefield on the screen
void mineDisplay_drawBoard(){
    //draws colums
    for(uint8_t i = 1; i<COL_NUM; i++){
        display_drawLine(GRID_COL_POS*i,SCREEN_EDGE,GRID_COL_POS*i,DISPLAY_HEIGHT,DISPLAY_WHITE);
    }
    //draws rows
    for(uint8_t i = 0; i<ROW_NUM;i++){
        display_drawLine(SCREEN_EDGE,GRID_ROW_POS*i,DISPLAY_WIDTH,GRID_ROW_POS*i,DISPLAY_WHITE);
    }
    return;
}

void mineDisplay_revealTile(uint8_t x, uint8_t y, uint8_t theatLvl){
    display_setCursor((GRID_COL_POS*x)+CHAR_OFFSET_X,(GRID_ROW_POS*y)+CHAR_OFFSET_Y);
    display_setTextSize(TEXT_SIZE);
    display_printChar(theatLvl);
    return;
}

void mineDisplay_drawFlag(uint8_t x,uint8_t y,bool erase){
    display_setTextSize(TEXT_SIZE);
    display_setCursor((GRID_COL_POS*x)+CHAR_OFFSET_X,(GRID_ROW_POS*y)+CHAR_OFFSET_Y);
    if (erase) display_setTextColor(DISPLAY_BLACK);
    else display_setTextColor(DISPLAY_DARK_YELLOW);
    display_printChar(FLAG_CHAR);
}

void fillTile(uint8_t x,uint8_t y,uint16_t color){
    display_fillRect(GRID_COL_POS*x,GRID_ROW_POS*y,GRID_ROW_POS,GRID_COL_POS, color);
    return;
}


void mineDisplay_drawMine(uint8_t x, uint8_t y, bool redBackground){
    // if the tile was chosen and is a bomb fill the background with red to indicate game over
    if(redBackground){
        fillTile(x,y,DISPLAY_RED);
        return;
    } 
    display_setTextSize(TEXT_SIZE);
    display_setCursor((GRID_COL_POS*x)+CHAR_OFFSET_X,(GRID_ROW_POS*y)+CHAR_OFFSET_Y);
    display_setTextColor(DISPLAY_RED);
    display_printChar(MINE_CHAR);
    return;
}

void mineDisplay_drawGameStart(bool erase){
    if (erase) display_setTextColor(DISPLAY_BLACK);
    else display_setTextColor(DISPLAY_BLUE);

    display_setTextSize(TITLE_TXT_SIZE);
    display_setCursor(TITLE_CURSOR_X,TITLE_CURSOR_Y);
    display_println(TITLE_MSG);

    display_setTextSize(TEXT_SIZE);
    display_setCursor(START_CURSOR_X,START_CURSOR_Y);
    display_println(START_MSG);

    if(!erase) display_setTextColor(DISPLAY_RED);
    display_setTextSize(INSTUCTION_SIZE);
    display_setCursor(FLAG_CURSOR_X,FLAG_CURSOR_Y);
    display_println(FLAG_MSG);
    display_setCursor(PRESS_CURSOR_X,PRESS_CURSOR_Y);
    display_println(PRESS_MSG);
    return;
}


