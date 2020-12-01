#include "Grid.h"

//Initialiaizes grid with empty values
Grid::Grid() {
    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            grid.at(i).at(j) = EMPTY;
        }
    }
}

//Generates random numbers to sudoku game
bool Grid::init_sudoku() {
    int try_count {0};
    std::array<char, GRID_SIZE> number_array;
    for(int i = 0; i < GRID_SIZE; i++) {
        reset_number_array(number_array);
        size_t index {0}, loop_counter {0};
        for(int j = 0; j < GRID_SIZE; j++) {
            Coor current_location = init_coordinates(j, i);
            if(check_if_value_exists(number_array.at(index), current_location) == NOT_FOUND) {
                grid.at(i).at(j) = number_array.at(index);
                number_array.at(index) = '0';
                loop_counter = 0;
            } else {
                j--;
                loop_counter++;
            }
            index++;
            reset_index(index);
            
            //skips the zeros in the number_array
            loop_through(index, number_array);
            
            if(loop_counter >= GRID_SIZE) {
                display_grid();
                //swaps with number_array.at(index) and swap_location
                find_replacement(number_array.at(index), current_location);
                try_count++;
                std::cout << std::endl;
                reset_index(index);
            }
            if(try_count >= GRID_SIZE)
                return false;
        }
        //display_grid();
    }
    return true;
}

void Grid::reset_grid() {
    for(int i = 0; i < GRID_SIZE; i++) {
        for(int j = 0; j < GRID_SIZE; j++) {
            grid.at(i).at(j) = EMPTY;
        }
    }
}

Coor Grid::init_coordinates(const int x, const int y) {
    Coor coordinates;
    coordinates.x = x;
    coordinates.y = y;
    return coordinates;
}

int Grid::check_if_value_exists(const char number, Coor current_location) {
    if(number == '0')
        return ZERO;
    //CHECK ROW
    for(int j = 0; j < GRID_SIZE; j++) {
        if(grid.at(current_location.y).at(j) == number) {
            return ROW;
        }
    }
    //CHECK COLUMN
    for(int i = 0; i < GRID_SIZE; i++) {
        if(grid.at(i).at(current_location.x) == number) {
            return COLUMN;
        }
    }

    //CHECK SECTION
    Coor section_begin = which_section(current_location);

    for(int i = section_begin.y; i < section_begin.y + SECTION; i++) {
        for(int j = section_begin.x; j < section_begin.x + SECTION; j++) {
            if(grid.at(i).at(j) == number) {
                return SECTION;
            }
        }
    }
    
    return NOT_FOUND;
}

Coor Grid::which_section(const Coor current_location) {
    Coor section_begin = init_coordinates(0, 0);
    int i {0}, j {0};
    int cur_section {1};
    while(cur_section <= 9) {
        for(i = section_begin.y; i < section_begin.y + SECTION; i++) {
            for(j = section_begin.x; j < section_begin.x + SECTION; j++) {
                if(current_location.y == i && current_location.x == j) {
                    return section_begin;
                }
            }
        }
        if(cur_section == 3 || cur_section == 6) {
            section_begin = init_coordinates(0, i);
        } else {
            section_begin = init_coordinates(j, i-3);
        }
        cur_section++;
    }
    return section_begin;
}

bool Grid::find_replacement(char &number, Coor current_location) {
    //checking in x axis
    if(check_row(number, current_location) == true) {
        return true;
    }
    return false;
}

bool Grid::check_row(char &number, Coor current_location) {
    for(int j {0}; j < current_location.x; j++) {
        Coor swap_location = init_coordinates(j, current_location.y);
        int match {0};
        char temp {grid.at(swap_location.y).at(j)};
        //check if swap_location is valid location for number
        if(check_if_value_exists(number, swap_location) == NOT_FOUND) {
            match++;
            //temporary EMPTY will be placed here during comparison
            grid.at(swap_location.y).at(j) = EMPTY;
        }
        //check if current location is valid location for swap_location number
        if(check_if_value_exists(temp, current_location) == NOT_FOUND) {
            match++;
        }
        grid.at(swap_location.y).at(j) = temp;
        if(match == 2) {
            swap<char>(grid.at(swap_location.y).at(j), number);
            std::cout << "SWAPPED: " << grid.at(swap_location.y).at(j)
            << " " << number << " COLUMN: " << current_location.y << std::endl;
            return true;
        }
    }
    return false;
}

void Grid::track_number(char &number, Coor &current_location) {
    grid.at(current_location.y).at(current_location.x) = number;
    int success {0};
    while(success < 2) {
        success = 0;
        if(track_column(number, current_location)) {
            number = replace_with_valid_value(current_location, COLUMN);
            grid.at(current_location.y).at(current_location.x) = number;
            success++;
        }
        if(track_row(number, current_location)) {
            number = replace_with_valid_value(current_location, ROW);
            grid.at(current_location.y).at(current_location.x) = number;
            success++;
        }
    }
    display_grid();
}

bool Grid::track_column(const char &number, Coor &current_location) {
    for(int i {0}; i < GRID_SIZE; i++) {
        if(grid.at(i).at(current_location.x) == number) {
            current_location = init_coordinates(current_location.x, i);
            return true;
        }
    }
    return false;
}

bool Grid::track_row(const char &number, Coor &current_location) {
    for(int j {0}; j < GRID_SIZE; j++) {
        if(grid.at(current_location.y).at(j) == number) {
            current_location = init_coordinates(j, current_location.y);
            return true;
        }
    }
    return false;
}

char Grid::replace_with_valid_value(Coor &current_location, const int direction) {
    std::array<char, GRID_SIZE> valid_values;
    init_from_one_to_nine(valid_values);
    for(int i {0}; i < GRID_SIZE; i++) {
        for(int j {0}; j < GRID_SIZE; j++) {
            switch (direction)
            {
            case COLUMN:
                if(grid.at(i).at(current_location.x) == valid_values.at(j)) {
                    valid_values.at(j) = '0';
                }
                break;
            case ROW:
                if(grid.at(current_location.y).at(i) == valid_values.at(j)) {
                    valid_values.at(j) = '0';
                }
                break;
            default:
                break;
            }
        }
    }
    size_t index {0};
    loop_through(index, valid_values);
    
    return valid_values.at(index);
}

void Grid::display_grid() const {
    for(size_t i = 0; i < grid.size(); i++) {
        for(size_t j = 0; j < grid.size(); j++) {
            std::cout << grid.at(i).at(j) << " ";
        }
        std::cout << std::endl;
    }
}

void Grid::reset_number_array(std::array<char, GRID_SIZE> &number_array) {
    init_from_one_to_nine(number_array);
    shuffle(number_array);
}

void Grid::init_from_one_to_nine(std::array<char, GRID_SIZE> &temp_array) {
    for(size_t i {0}; i < temp_array.size(); i++) {
        temp_array.at(i) = static_cast<char>(i+1)+48;
    }
}

void Grid::shuffle(std::array<char, GRID_SIZE> &number_array) {
    size_t i = 0;
    while(i < number_array.size()) {
        size_t random_index = rand() % GRID_SIZE;
        swap<char>(number_array.at(i), number_array.at(random_index));
        i++;
    }
}

bool Grid::reset_index(size_t &index) {
    if(index >= GRID_SIZE) {
        index = 0;
        return true;
    }
    return false;
}

void Grid::loop_through(size_t &index, const std::array<char, GRID_SIZE> array) {
    int loops {0};
    while(array.at(index) == '0') {
        index++;
        if(reset_index(index) == true) {
            loops++;
        }
        if(loops == 2)
            break;
    }
}

template<typename T>
void Grid::swap(T &i, T &j) {
    T temp = i;
    i = j;
    j = temp;

}