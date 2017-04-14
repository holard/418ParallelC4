
#include "SeqStateScorer.h"
#include <vector>

using boardVec = std::vector<Board>;

void getMoves(Board& input, int player, boardVec& result) {
    for (int col = 0; col < COLS; col++) {
        if (input.state[ROWS-1][col] == 0) {
            int nextrow = 0;
            while (input.state[nextrow][col] != 0) {
                nextrow++;
            }
            Board newboard;
            newboard.state[nextrow][col] = player;
            result.push_back(newboard);
        }
    }
}

int SeqStateScorer::searchToDepth(Board& initialState, int player, int depth) {
    boardVec bv;
    getMoves(initialState, 1, bv);
    for (auto board : bv) {
        board.print();
    }
    return 0;
}
