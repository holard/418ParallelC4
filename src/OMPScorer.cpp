
#include "OMPScorer.h"
#include <vector>
#include <unordered_map>
#include <iostream>
#include <omp.h>

void deposit(Frontier& input, boardVec vec) {
    int num = vec.size();
    int index = __sync_fetch_and_add(&input.count, num);
    for (int i = 0; i < num; i++) {
        input.buffer[index+i] = vec[i];
    }
}

void score_base (Frontier& input, int player, boardMap& result) {

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
                if (result.count(k) == 0)
                    keys.push_back(k);
            }
            #pragma omp critical (map)
            {
                for (int j = 0; j < keys.size(); j++) {
                    Key k = keys[j];
                    int ks = scores[start+j];
                    result[k] = ks;
                }
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
            result[k] = ks;
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

void score_frontier (Frontier& input, int player, int depth, boardMap& result) {

    if (depth == 0) {
        score_base (input, player, result);
        return;
    }
    boardMap memo;
    {
        Frontier next(input.count * COLS);

        #pragma omp parallel for
        for (int i = 0; i < input.count; i++) {
            Board& b = input.buffer[i];
            int sc = b.score();
            if (abs(sc) == INF) {
                Key k = b.getKey();
                if (result.count(k) == 0) {
                    #pragma omp critical (result)
                    {
                        result[k] = sc;
                    }
                }
                continue;
            }
            boardVec bv;
            getMoves(b, player, bv);
            deposit(next, bv);
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
            int s = memo[key];
            if (s*player > best*player) {
                best = s;
            }
        }
        if (result.count(k) > 0)
            continue;
        #pragma omp critical (result)
        {
            result[k] = best;
        }
    }
    //std::cout << "States searched: " << result.size() << "\n";
}

// returns the column to play in
int OMPScorer::searchToDepth(Board& initialState, int player, int depth) {
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
        int bscore = memo[k];
        //std::cout << "score: " << bscore << "\n";
        if (bscore*player >= best*player) {
            best = bscore;
            bestcol = col;
        }

    }
    return bestcol;
}
