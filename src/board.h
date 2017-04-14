
#include <iostream>
#include <sstream>
#define COLS 7
#define ROWS 7

class Board {
    public:
        int state[ROWS][COLS];
        Board() : state() {}
        void print() {
            for (int row = ROWS-1; row >= 0; row--) {
                std::ostringstream str;
                for (int col = 0; col < COLS; col++) {
                    str << "|" << state[row][col];
                }
                std::cout << str.str() << "|\n";
            }
            std::cout << "-------\n";
        }

};


