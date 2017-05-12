
#include "board.h"

class SeqStateScorer {
    public:
        // returns the best column to play in
        int searchDFS(Board& initialState, int player, int depth);
        int searchBFS(Board& initialState, int player, int depth);
};

