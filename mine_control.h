#include <stdio.h>
#include <stdint.h>

#ifndef MINE_CONTROL_H_
#define MINE_CONTROL_H_

#define FEILD_MAX_ROWS 10
#define FEILD_MAX_COL 10


//The game is made of this field that is consisted of rows and colums of tiles that can either
//be a mine or an empty place.
typedef struct 
{
    bool is_revealeud; //bool to check weather the tile has been selected and shown
    bool is_mine; //bool to check weather tile is a mine
    uint8_t x_pos; // Where the tile is on the x plane
    uint8_t y_pos; // Where the tile is on the y plane
    uint8_t threat_value; //this will be the value indicating if a mine is close to the tile
    tile* neighbor[]; //neighbors are the adjecent tiles next to each other.
    //have to think about this one.^

}tile;

//The game board data structure.
typedef struct {
    tile squars[FEILD_MAX_ROWS][FEILD_MAX_COL]; //the field is just made up of tiles that are squares
}mine_field;

//This is the selected tile that a player will choose whice is defined by a row and coloum.
typedef struct {
    uint8_t row;
    uint8_t col;
}selected_tile;



//init funciton for the mineControl. Used before all other functions.
void mineControl_init();

//normal tickfunction for the mineSweeper game top level
void mineControl_tick();





#endif /*mine_control.h*/