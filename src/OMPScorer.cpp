
#include "OMPScorer.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <omp.h>

#define PARALLEL_THRESHOLD 0

void score_frontier (Frontier& input, int player, int depth, LocklessMap& result);


void score_base (Frontier& input, int player, LocklessMap& result) {

    int local_size = 32;
    int padding = 32;
    int maxthreads = omp_get_max_threads();
    int* scores = new int[maxthreads * (local_size + padding)];
    int* indices = new int[maxthreads * (local_size + padding)];
    int* sizes = new int[maxthreads]();

#pragma omp parallel for
    for (int i = 0; i < input.count; i++) {
        Board& b = input.buffer[i];
        int tnum = omp_get_thread_num();
        int s = b.score();
        int l_count = sizes[tnum];
        int start = tnum * (local_size + padding);
        scores[start + l_count] = s;
        indices[start + l_count] = i;
        sizes[tnum] += 1;
        if (l_count + 1 == local_size) {
            std::vector<Key> keys;
            for (int j = 0; j < local_size; j++) {
                Key k = input.buffer[indices[start+j]].getKey();
                int ks = scores[start+j];
                result.put(k, ks);
            }
            sizes[tnum] = 0;
        }
    }
    for (int tnum = 0; tnum < maxthreads; tnum++) {
        int start = tnum * (local_size + padding);
        int l_count = sizes[tnum];
        for (int j = 0; j < l_count; j++) {
            Key k = input.buffer[indices[start+j]].getKey();
            int ks = scores[start+j];
            result.put(k, ks);
        }
    }
}

void basic_base (Frontier& input, int player, boardMap& result) {

#pragma omp parallel for
    for (int i = 0; i < input.count; i++) {
        Board& b = input.buffer[i];
        Key k = b.getKey();
        int s = b.score();
#pragma omp critical (map) 
        {
            result[k] = s;
        }
    }
    std::cout << "States searched: " << result.size() << "\n";
}

void score_seq (Frontier& input, int player, int depth, LocklessMap& result) {

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
void score_frontier (Frontier& input, int player, int depth, LocklessMap& result) {

    if (depth == 0) {
        score_base (input, player, result);
        return;
    }
    if (input.count < PARALLEL_THRESHOLD) {
        score_seq(input, player, depth, result);
        return;
    }
    LocklessMap memo(input.count * 7);
    LocklessMap added(input.count * 7);
    {
        Frontier next(input.count * COLS);

        #pragma omp parallel for
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
            boardVec dep;
            for (auto board : bv) {
                Key k = board.getKey();
                if (added.put(k,1)) {
                    dep.push_back(board);
                }
            }
            deposit(next, dep);
        }
        score_frontier(next, -player, depth-1, memo);
    }
    #pragma omp parallel for
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
            int s = memo.get(key);
            if (s*player > best*player) {
                best = s;
            }
        }
        result.put(k, best);
    }
    //std::cout << "States searched: " << result.size() << "\n";
}

// returns the column to play in
int OMPScorer::searchToDepth(Board& initialState, int player, int depth) {
    LocklessMap memo(COLS);

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
