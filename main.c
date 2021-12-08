#include "mineDisplay.h"
#include "mineControl.h"
#include <stdlib.h>

int main() {
    mineDisplay_init();

    srand(12);
    mineDisplay_drawBoard();
    setMines();

    return 0;
}

void isr_function() {

}