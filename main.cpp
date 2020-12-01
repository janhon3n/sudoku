#include "Grid.h"

int main() {
    srand(time(NULL));
    Grid grid;
    int counter {0};
    while(true) {
        if(grid.init_sudoku() == true)
            break;
        grid.reset_grid();
        counter++;
    }
    grid.display_grid();
    std::cout << "RESET COUNT: " << counter << std::endl;
    return 0;
}