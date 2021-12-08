#include "mineDisplay.h"

int main() {
    mineDisplay_init();

    mineDisplay_drawBoard();
    for (uint8_t i = 0; i < 8; i++) 
        mineDisplay_revealTile(i,0,i);
    mineDisplay_revealTile(0,1,8);
    mineDisplay_drawFlag(4,3, false);
    mineDisplay_drawFlaggedDirt(4,3);

    mineDisplay_drawMine(3, 3, true);
    mineDisplay_drawMine(2, 2, false);

    
    mineDisplay_drawFlag(4,4,false);

    return 0;
}

void isr_function() {

}