#include "Grid.h"

int Grid::track_number_counter = 0;

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
    std::array<char, GRID_SIZE> number_array;
    for(int i = 0; i < GRID_SIZE; i++) {
        reset_number_array(number_array);
        size_t index {0}, fail_counter {0};
        size_t replacement_count {0};
        for(int j = 0; j < GRID_SIZE; j++) {
            Coor current_location = init_coordinates(j, i);
            if(check_if_value_exists(number_array.at(index), current_location) == NOT_FOUND) {
                add_to_grid(number_array.at(index), current_location);
                fail_counter = 0;
            } else {
                j--;
                fail_counter++;
            }
            index++;
            //If index if greater or equal than 9 then index resets to zero
            reset_index(index);
            
            //skips the zeros in the number_array
            loop_through(index, number_array);
            
            if(fail_counter >= GRID_SIZE) {
                if(!find_replacement(number_array.at(index), current_location)) {
                    replacement_count++;
                    if(replacement_count >= GRID_SIZE) {
                        track_number(number_array.at(index), current_location);
                        if(get_track_number_counter() == TNC)
                            return false;
                        j++;
                        if(loop_through(index, number_array) == true)
                            break;
                        replacement_count = 0;
                    }
                }
            }
        }
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
    if(scan_row(number, current_location))
        return ROW;
    //CHECK COLUMN
    if(scan_column(number, current_location))
        return COLUMN;
    
    //CHECK SECTION
    Coor section_begin = which_section(current_location);
    if(scan_section(number, section_begin, current_location))
        return SECTION;
    
    return NOT_FOUND;
}

bool Grid::scan_row(const char number, Coor current_location) {
    for(int j = 0; j < GRID_SIZE; j++) {
        if(grid.at(current_location.y).at(j) == number && j != current_location.x) {
            return true;
        }
    }
    return false;
}

bool Grid::scan_column(const char number, Coor current_location) {
    for(int i = 0; i < GRID_SIZE; i++) {
        if(grid.at(i).at(current_location.x) == number && i != current_location.y) {
            return true;
        }
    }
    return false;
}

bool Grid::scan_section(const char number, const Coor section_begin, const Coor current_location) {
    for(int i = section_begin.y; i < section_begin.y + SECTION; i++) {
        for(int j = section_begin.x; j < section_begin.x + SECTION; j++) {
            if(grid.at(i).at(j) == number) {
                if(i != current_location.y && j != current_location.x) {
                    return true;
                }
            }
        }
    }
    return false;
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
            return true;
        }
    }
    return false;
}

void Grid::add_to_grid(char &number, const Coor current_location) {
    grid.at(current_location.y).at(current_location.x) = number;
    number = '0';
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

bool Grid::loop_through(size_t &index, const std::array<char, GRID_SIZE> array) {
    size_t loops {0};
    while(array.at(index) == '0') {
        index++;
        if(reset_index(index) == true) {
            loops++;
        }
        if(loops >= 2) {
            //All of the elements are 0
            return true;
        }
    }
    return false;
}

template<typename T>
void Grid::swap(T &i, T &j) {
    T temp = i;
    i = j;
    j = temp;
}

void Grid::track_number(char number, Coor current_location) {
    if(get_track_number_counter() >= TNC) {
        std::cout << "Track_number called: " << get_track_number_counter() << " times." << std::endl;
        return;
    }
    track_number_counter++;
    if(check_if_value_exists(number, current_location) == NOT_FOUND) {
        std::cout << "Track_number called: " << get_track_number_counter() << " times." << std::endl;
        return;
    } else {
        grid.at(current_location.y).at(current_location.x) = number;

        Coor section_begin = which_section(current_location);
        if(scan_section(number, section_begin, current_location)) {
            Coor left_of = current_location;
            track_section(number, section_begin, current_location);
            track_number(number, current_location);
            current_location = left_of;
            number = grid.at(current_location.y).at(current_location.x);
        }

        if(scan_column(number, current_location)) {
            Coor left_of = current_location;
            track_column(number, current_location);
            track_number(number, current_location);
            current_location = left_of;
            number = grid.at(current_location.y).at(current_location.x);
        }

        if(scan_row(number, current_location)) {
            Coor left_of = current_location;
            track_row(number, current_location);
            track_number(number, current_location);
            current_location = left_of;
            number = grid.at(current_location.y).at(current_location.x);
        }
    }
}

bool Grid::track_column(char &number, Coor &current_location) {
    //initializes useable_values array
    std::array<char, GRID_SIZE> useable_values;
    init_from_one_to_nine(useable_values);
    for(int i {0}; i < GRID_SIZE; i++) {
        for(int j {0}; j < GRID_SIZE; j++) {
            if(grid.at(i).at(current_location.x) == useable_values.at(j)) {
                useable_values.at(j) = '0';
            }
        }
    }

    //Changes the current_location cordinates
    for(int i {0}; i < GRID_SIZE; i++) {
        if(grid.at(i).at(current_location.x) == number && i != current_location.y) {
            current_location = init_coordinates(current_location.x, i);
            break;
        }
    }
    size_t index {0};
    //loop_through(index, useable_values);
    while(useable_values.at(index) == '0') {
        index = rand() % GRID_SIZE;
    }
    
    grid.at(current_location.y).at(current_location.x) = useable_values.at(index);
    number = grid.at(current_location.y).at(current_location.x);
    return true;
}

bool Grid::track_row(char &number, Coor &current_location) {
    //intializes useable_values
    std::array<char, GRID_SIZE> useable_values;
    init_from_one_to_nine(useable_values);
    for(int j {0}; j < GRID_SIZE; j++) {
        for(int i {0}; i < GRID_SIZE; i++) {
            if(grid.at(current_location.y).at(j) == useable_values.at(i))
                useable_values.at(i) = '0';
        }
    }

    //Changes the current_location cordinates
    for(int j {0}; j < GRID_SIZE; j++) {
        if(grid.at(current_location.y).at(j) == number && j != current_location.x) {
            current_location = init_coordinates(j, current_location.y);
            break;
        }
    }
    size_t index {0};
    //loop_through(index, useable_values);

    while(useable_values.at(index) == '0') {
        index = rand() % GRID_SIZE;
    }

    grid.at(current_location.y).at(current_location.x) = useable_values.at(index);
    number = grid.at(current_location.y).at(current_location.x);
    return true;
}

bool Grid::track_section(char &number, const Coor &section_begin, Coor &current_location) {
    //initializes useable_values array
    std::array<char, GRID_SIZE> useable_values;
    init_from_one_to_nine(useable_values);
    size_t index {0};
    for(int i {section_begin.y}; i < section_begin.y + SECTION; i++) {
        for(int j {section_begin.x}; j < section_begin.x + SECTION; j++) {
            index = 0;
            while(index < GRID_SIZE) {
                if(grid.at(i).at(j) == useable_values.at(index)) {
                    useable_values.at(index) = '0';
                }
                index++;
            }
        }
    }

    //Changes the current_location cordinates
    for(int i {section_begin.y}; i < section_begin.y + SECTION; i++) {
        bool exit_loop {false};
        for(int j {section_begin.x}; j < section_begin.x + SECTION; j++) {
            if(grid.at(i).at(j) == number) {
                if(i != current_location.y && j != current_location.x) {
                    current_location = init_coordinates(j, i);
                    exit_loop = true;
                    break;
                }
            }
        }
        if(exit_loop)
            break;
    }
    index = 0;
    //loop_through(index, useable_values);

    while(useable_values.at(index) == '0') {
        index = rand() % GRID_SIZE;
    }

    grid.at(current_location.y).at(current_location.x) = useable_values.at(index);
    number = grid.at(current_location.y).at(current_location.x);
    return true;
}
