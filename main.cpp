#include "Grid.h"

int main() {
    srand(time(nullptr));
    Grid grid;
    while(grid.init_sudoku() == false) {
        grid.reset_grid();
    }
    grid.display_grid();
    return 0;
}