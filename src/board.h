
#ifndef BOARD_DEF
#define BOARD_DEF

#include <iostream>
#include <sstream>
#include <string>
#define COLS 7
#define ROWS 7
#define INF 32767

static char mapPiece(int n) {
    if (n == 0) {
        return '_';
    }
    if (n == 1) {
        return 'O';
    }
    return 'X';
}

class Board {
    public:
        int state[ROWS][COLS];
        Board() : state() {}

        Board(const Board &other) {
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    state[row][col] = other.state[row][col];
                }
            }
        }

        void print() {
            for (int row = ROWS-1; row >= 0; row--) {
                std::ostringstream str;
                for (int col = 0; col < COLS; col++) {
                    str << "|" << mapPiece(state[row][col]);
                }
                std::cout << str.str() << "|\n";
            }
            std::cout << "-------\n";
        }

        int get(int row, int col) {
            if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
                return 0;
            }
            return state[row][col];
        }

        std::string getKey() {
            std::ostringstream str;
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    str << state[row][col];
                }
            }
            return str.str();
        }

        // Uses a heuristic to score a board
        int score() {
            int sum = 0;
            for (int row = 0; row < ROWS; row++) {
                for (int col = 0; col < COLS; col++) {
                    int p = state[row][col];
                    if (p == 0)
                        continue;
                    int length = 1;
                    while (get(row,col+length) == p) {
                        length += 1;
                    }
                    if (length >= 4)
                        return p*INF;
                    sum += p*length;
                    length = 1;
                    while (get(row+length, col) == p) {
                        length += 1;
                    }
                    if (length >= 4)
                        return p*INF;
                    sum += p*length;
                    length = 1;
                    while (get(row+length, col+length) == p) {
                        length += 1;
                    }
                    if (length >= 4)
                        return p*INF;
                    sum += p*length;
                    length = 1;
                    while (get(row+length, col-length) == p) {
                        length += 1;
                    }
                    if (length >= 4)
                        return p*INF;
                    sum += p*length;
                }
            }
            return sum;
        }

};

#endif

