
#include "board.h"

class OMPScorer {
    public:
        // returns the best column to play in
        int searchToDepth(Board& initialState, int player, int depth);
};

class Frontier {
    public:
    int max_size;
    int count;
    Board* buffer;
    Frontier(int size) {max_size = size; count = 0; buffer = new Board[size];};
};

