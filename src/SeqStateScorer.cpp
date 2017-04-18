
#include "SeqStateScorer.h"
#include <vector>
#include <unordered_map>
#include <iostream>

using boardVec = std::vector<Board>;
using Key = std::string;
using boardMap = std::unordered_map<Key, int>;

int score(Board& input, int player, int depth, boardMap& memo) {
    Key key = input.getKey();
    if (memo.count(key) > 0) {
        return memo[key];
    }

    int sc = input.score();
    if (abs(sc) == INF || depth <= 0) {
        memo[key] = sc;
        return sc;
    }
    boardVec bv;
    getMoves(input, player, bv);

    // no moves available
    if (bv.size() == 0) {
        memo[key] = sc;
        return sc;
    }

    int best = player * INF * -1;
    for (auto board : bv) {
        int s = score(board, -player, depth - 1, memo);
        if (s*player > best*player) {
            best = s;
        }
    }
    memo[key] = best;
    return best;
}

// returns the column to play in
int SeqStateScorer::searchToDepth(Board& initialState, int player, int depth) {
    boardVec bv;
    getMoves(initialState, player, bv);
    boardMap memo;
    int best = player * INF * -1;
    int bestcol = -1;
    int i = 0;
    for (int col = 0; col < COLS; col++) {
        if (initialState.state[ROWS-1][col] != 0)
            continue;
        Board& board = bv[i];
        i++;
        //board.print();
        int bscore = score(board, -player, depth-1, memo);
        //std::cout << "score: " << bscore << "\n";
        if (bscore*player >= best*player) {
            best = bscore;
            bestcol = col;
        }

    }
    std::cout << "States searched: " << memo.size() << "\nNum moves: " << bv.size() << "\n";
    return bestcol;
}
