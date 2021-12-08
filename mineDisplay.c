#include "mineDisplay.h"
#include "mineControl.h"
#include "display.h"
#include "buttons.h"
#include <stdio.h>
#include <stdint.h>

#define LINE_COLOR          DISPLAY_BLACK
#define REVEALED_TILE_COLOR DISPLAY_GRAY
#define BACKGROUND_COLOR    DISPLAY_LIGHT_GRAY
#define FLAG_COLOR          DISPLAY_YELLOW
#define MINE_COLOR          DISPLAY_BLACK


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


#define NUM_COL       MINE_CONTROL_NUM_COLS
#define NUM_ROW       MINE_CONTROL_NUM_ROWS
#define ROW_HEIGHT    (DISPLAY_HEIGHT / NUM_ROW)
#define COLUMN_WIDTH  (DISPLAY_WIDTH / NUM_COL)
#define CHAR_OFFSET_X ((COLUMN_WIDTH-(DISPLAY_CHAR_WIDTH-1)*TEXT_SIZE)/2)
#define CHAR_OFFSET_Y ((ROW_HEIGHT-DISPLAY_CHAR_WIDTH*TEXT_SIZE)/2)
#define SCREEN_EDGE 0

#define FLAG_CHAR 'F'
#define MINE_CHAR 'M'

const uint16_t numberColors[] = {REVEALED_TILE_COLOR, DISPLAY_BLUE, DISPLAY_RED, DISPLAY_GREEN, DISPLAY_DARK_BLUE, DISPLAY_DARK_RED, DISPLAY_DARK_CYAN, DISPLAY_BLACK, DISPLAY_DARK_GRAY};

//init the main display and fills it with black screen
void mineDisplay_init(){
    display_init();
    display_fillScreen(BACKGROUND_COLOR);
    return;
}

// helper function to fill tiles with a color!
void fillTile(uint8_t x,uint8_t y,uint16_t color){
    display_fillRect(COLUMN_WIDTH*x + 1,ROW_HEIGHT*y + 1,COLUMN_WIDTH - 1,ROW_HEIGHT - 1, color);
    return;
}

//draws the minefield on the screen
void mineDisplay_drawBoard(){
    //draws colums
    for(uint8_t i = 1; i<NUM_COL; i++){
        display_drawLine(COLUMN_WIDTH*i,SCREEN_EDGE,COLUMN_WIDTH*i,DISPLAY_HEIGHT,LINE_COLOR);
    }
    //draws rows
    for(uint8_t i = 1; i<NUM_ROW;i++){
        display_drawLine(SCREEN_EDGE,ROW_HEIGHT*i,DISPLAY_WIDTH,ROW_HEIGHT*i,LINE_COLOR);
    }
    return;
}

void mineDisplay_revealTile(uint8_t x, uint8_t y, uint8_t threatLvl){
    fillTile(x,y,REVEALED_TILE_COLOR);
    display_setCursor((COLUMN_WIDTH*x)+CHAR_OFFSET_X,(ROW_HEIGHT*y)+CHAR_OFFSET_Y);
    display_setTextSize(TEXT_SIZE);
    display_setTextColor(numberColors[threatLvl]);
    display_printDecimalInt(threatLvl);
    return;
}

void mineDisplay_drawFlag(uint8_t x,uint8_t y,bool erase){
    display_setTextSize(TEXT_SIZE);
    display_setCursor((COLUMN_WIDTH*x)+CHAR_OFFSET_X,(ROW_HEIGHT*y)+CHAR_OFFSET_Y);
    if (erase) display_setTextColor(BACKGROUND_COLOR);
    else display_setTextColor(FLAG_COLOR);
    display_printChar(FLAG_CHAR);
}


void mineDisplay_drawMine(uint8_t x, uint8_t y, bool redBackground){
    display_setTextColor(DISPLAY_RED);
    // if the tile was chosen and is a bomb fill the background with red to indicate game over
    if(redBackground){
        fillTile(x,y,DISPLAY_RED);
    }
    else {
        fillTile(x,y,REVEALED_TILE_COLOR);
    }
    display_setTextSize(TEXT_SIZE);
    display_setCursor((COLUMN_WIDTH*x)+CHAR_OFFSET_X,(ROW_HEIGHT*y)+CHAR_OFFSET_Y);
    display_setTextColor(MINE_COLOR);    
    display_printChar(MINE_CHAR);
    return;
}

void mineDisplay_drawGameStart(bool erase){
    if (erase) display_setTextColor(BACKGROUND_COLOR);
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

//draws a flagged piece of dirt once the game is over (red x in some versions)
void mineDisplay_drawFlaggedDirt(uint8_t x, uint8_t y) {
    display_setCursor((COLUMN_WIDTH*x)+CHAR_OFFSET_X,(ROW_HEIGHT*y)+CHAR_OFFSET_Y);
    display_setTextColor(DISPLAY_RED);
    display_printChar('X');
}

