
#include "board.h"

class SeqStateScorer {
    public:
        // returns the best column to play in
        int searchToDepth(Board& initialState, int player, int depth);
};

