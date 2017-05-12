
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
int SeqStateScorer::searchDFS(Board& initialState, int player, int depth) {
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

void score_base (Frontier& input, int player, boardMap& result) {

    for (int i = 0; i < input.count; i++) {
        Board& b = input.buffer[i];
        Key k = b.getKey();
        int s = b.score();
        result[k] = s;
    }
}
void score_frontier (Frontier& input, int player, int depth, boardMap& result) {

    if (depth == 0) {
        score_base (input, player, result);
        return;
    }
    boardMap memo;
    boardMap added;
    {
        Frontier next(input.count * COLS);

        for (int i = 0; i < input.count; i++) {
            Board& b = input.buffer[i];
            int sc = b.score();
            if (abs(sc) == INF) {
                Key k = b.getKey();
                result[k] = sc;
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
        score_frontier(next, -player, depth-1, memo);
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
            int s = memo[key];
            if (s*player > best*player) {
                best = s;
            }
        }
        if (result.count(k) > 0)
            continue;
        result[k] = best;
    }
}

int SeqStateScorer::searchBFS(Board& initialState, int player, int depth) {
    boardMap memo;

    Frontier next(COLS);
    boardVec bv;
    getMoves(initialState, player, bv);
    deposit(next, bv);
    score_frontier(next, -player, depth-1, memo);
    
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
        int bscore = memo[k];
        //std::cout << "score: " << bscore << "\n";
        if (bscore*player >= best*player) {
            best = bscore;
            bestcol = col;
        }

    }
    std::cout << "States searched: " << memo.size() << "\nNum moves: " << bv.size() << "\n";
    return bestcol;
}

