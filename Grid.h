#ifndef _GRID_H_
#define _GRID_H_
#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>
#include <stack>

#define EMPTY ' '
#define GRID_SIZE 9

#define ZERO 0
#define ROW 1
#define COLUMN 2
#define SECTION 3
#define NOT_FOUND 4

typedef struct coordinates {
    int x, y;
}Coor;

class Grid {
    private:
        std::array<std::array<char, GRID_SIZE>, GRID_SIZE> grid;
    public:
        Grid();
        ~Grid() = default;

        //GETTERS AND SETTERS

        bool init_sudoku();
        void reset_grid();
        Coor init_coordinates(const int x, const int y);
        int check_if_value_exists(const char number, Coor current_location);
        Coor which_section(const Coor current_location);
        bool find_replacement(char &number, Coor current_location);
        bool check_row(char &number, Coor current_location);

        char replace_with_valid_value(Coor &current_location, const int direction);

        void reset_number_array(std::array<char, GRID_SIZE> &number_array);
        void init_from_one_to_nine(std::array<char, GRID_SIZE> &temp_array);
        void shuffle(std::array<char, GRID_SIZE> &number_array);
        bool reset_index(size_t &index);
        void loop_through(size_t &index, const std::array<char, GRID_SIZE> array);

        //REPLACE FUNCTION

        template<typename T>
        void swap(T &i, T &j);

        void display_grid() const;
};

#endif