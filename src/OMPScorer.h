
#include "LocklessMap.h"
#include "board.h"

class OMPScorer {
    public:
        // returns the best column to play in
        int searchBFS(Board& initialState, int player, int depth);
        int searchDFS(Board& initialState, int player, int depth);
};

