#ifndef _GRID_H_
#define _GRID_H_
#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>

#define EMPTY ' '
#define GRID_SIZE 9

#define ZERO 0
#define ROW 1
#define COLUMN 2
#define SECTION 3
#define NOT_FOUND 4

#define TNC 2000

typedef struct coordinates {
    int x, y;
}Coor;

class Grid {
    private:
        std::array<std::array<char, GRID_SIZE>, GRID_SIZE> grid;
        static int track_number_counter;
    public:
        Grid();
        ~Grid() = default;

        //GETTERS AND SETTERS
        static int get_track_number_counter() {return track_number_counter;}

        bool init_sudoku();
        void reset_grid();
        Coor init_coordinates(const int x, const int y);

        int check_if_value_exists(const char number, Coor current_location);
        bool scan_row(const char number, Coor current_location);
        bool scan_column(const char number, Coor current_location);
        bool scan_section(const char number, const Coor section_begin, const Coor current_location);

        Coor which_section(const Coor current_location);
        bool find_replacement(char &number, Coor current_location);
        bool check_row(char &number, Coor current_location);

        void add_to_grid(char &number, const Coor current_location);

        void reset_number_array(std::array<char, GRID_SIZE> &number_array);
        void init_from_one_to_nine(std::array<char, GRID_SIZE> &temp_array);
        void shuffle(std::array<char, GRID_SIZE> &number_array);
        bool reset_index(size_t &index);
        bool loop_through(size_t &index, const std::array<char, GRID_SIZE> array);

        //REPLACE FUNCTION

        template<typename T>
        void swap(T &i, T &j);

        void display_grid() const;

        void track_number(char number, Coor current_location);
        bool track_column(char &number, Coor &current_location);
        bool track_row(char &number, Coor &current_location);
        bool track_section(char &number, const Coor &section_begin, Coor &current_location);
};

#endif