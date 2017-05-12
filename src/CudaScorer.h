
#include "LocklessMap.h"
#include "board.h"


class CudaScorer {
    public:
        // returns the best column to play in
        int searchBFS(Board& initialState, int player, int depth);
};


