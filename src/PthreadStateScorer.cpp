
#include "PthreadStateScorer.h"
#include "LocklessMap.h"
#include <vector>
#include <unordered_map>
#include <iostream>

#define NUM_THREADS 7

using boardVec = std::vector<Board>;
using Key = std::string;
using boardMap = std::unordered_map<Key, int>;

struct threadargs{Board& board; int player; int depth; LocklessMap& memo; int* result;};

int pthread_score(Board& input, int player, int depth, LocklessMap& memo) {
    Key key = input.getKey();
    if (memo.count(key) > 0) {
        return memo.get(key);
    }

    int sc = input.score();
    if (abs(sc) == INF || depth <= 0) {
        memo.put(key,sc);
        return sc;
    }
    boardVec bv;
    getMoves(input, player, bv);

    // no moves available
    if (bv.size() == 0) {
        memo.put(key,sc);
        return sc;
    }

    int best = player * INF * -1;
    for (auto board : bv) {
        int s = pthread_score(board, -player, depth - 1, memo);
        if (s*player > best*player) {
            best = s;
        }
    }
    memo.put(key,best);
    return best;
}

void *thread_func(void *arg) {
  threadargs* myargs = (threadargs*)(arg);
  int bscore = pthread_score(myargs->board, myargs->player, myargs->depth, myargs->memo);
  *(myargs->result) = bscore;

}

// returns the column to play in
int PthreadStateScorer::searchToDepth(Board& initialState, int player, int depth) {
    boardVec bv;
    getMoves(initialState, player, bv);
    LocklessMap memo(80000);
    int best = player * INF * -1;
    int bestcol = -1;
    int i = 0;
    int results[COLS];
    int itocol[COLS];

    pthread_t threads[NUM_THREADS];

    for (int col = 0; col < COLS; col++) {
        if (initialState.state[ROWS-1][col] != 0)
            continue;
        Board& board = bv[i];
        itocol[i] = col;
        //board.print();
        threadargs args = {board, -player, depth-1, memo, (results+i)};
        int r = pthread_create(&threads[i], NULL, thread_func, (void *)&args);
        i++;
        //int bscore = score(board, -player, depth-1, memo);
        //std::cout << "score: " << bscore << "\n";
    }

    for (int j = 0; j < i; j++){
      pthread_join(threads[j], NULL);
        //after join threads, outside for loop
        int bscore = results[j];
        if (bscore*player >= best*player) {
            best = bscore;
            bestcol = itocol[j];
        }
    }
    std::cout << "States searched: " /*<< memo.size()*/ << "\nNum moves: " << bv.size() << "\n";
    return bestcol;
}
