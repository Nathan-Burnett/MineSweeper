#include "mineDisplay.h"
#include "mineControl.h"
#include <stdlib.h>

int main() {
    mineDisplay_init();

    mineDisplay_drawBoard();
    for (uint8_t i = 0; i < 9; ++i) mineDisplay_revealTile(i, 0 , i);

    return 0;
}

void isr_function() {

}