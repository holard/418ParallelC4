
#include "CudaScorer.h"

int kernel();
void score_base_cuda (Frontier& input, int player, LocklessMap& result);

// pure sequential scoring
void score_frontier_cuda (Frontier& input, int player, int depth, LocklessMap& result) {
    if (depth == 0) {
        // Use kernel for base case
        score_base_cuda (input, player, result);
        return;
    }

    LocklessMap memo(input.count * 7);
    boardMap added;
    {
        Frontier next(input.count * COLS);

        for (int i = 0; i < input.count; i++) {
            Board& b = input.buffer[i];
            int sc = b.score();
            if (abs(sc) == INF) {
                Key k = b.getKey();
                result.put(k, sc);
                continue;
            }
            boardVec bv;
            getMoves(b, player, bv);
            for (auto board : bv) {
                Key k = board.getKey();
                if (added.count(k) == 0) {
                    next.buffer[next.count++] = board;
                    added[k] = 1;
                }
            }
        }
        score_frontier_cuda(next, -player, depth-1, memo);
    }
    for (int i = 0; i < input.count; i++) {
        Board& b = input.buffer[i];
        Key k = b.getKey();
        if (result.count(k) > 0) {
            continue;
        }
        boardVec bv;
        getMoves(b, player, bv);
        int best = player * INF * -1;
        for (auto board : bv) {
            Key key = board.getKey();
            if (memo.count(key) == 0) {
                return;
            }
            int s = memo.get(key);
            if (s*player > best*player) {
                best = s;
            }
        }
        if (result.count(k) > 0)
            continue;
        result.put(k, best);
    }
    //std::cout << "States searched: " << result.size() << "\n";
}

// returns the column to play in
int CudaScorer::searchToDepth(Board& initialState, int player, int depth) {
    LocklessMap memo(COLS);

    Frontier next(COLS);
    boardVec bv;
    getMoves(initialState, player, bv);
    deposit(next, bv);
    score_frontier_cuda(next, -player, depth-1, memo);
    
    int best = player * INF * -1;
    int bestcol = -1;
    int i = 0;
    for (int col = 0; col < COLS; col++) {
        if (initialState.state[ROWS-1][col] != 0)
            continue;
        Board& board = bv[i];
        i++;
        //board.print();
        Key k = board.getKey();
        if (memo.count(k) == 0)
            std::cout << "Key " << k << " not found!\n";
        int bscore = memo.get(k);
        //std::cout << "score: " << bscore << "\n";
        if (bscore*player >= best*player) {
            best = bscore;
            bestcol = col;
        }

    }
    std::cout << "score: " << best << "\n";
    return bestcol;
}

