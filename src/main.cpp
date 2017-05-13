#include "OMPScorer.h"
#include "SeqStateScorer.h"
#include "PthreadStateScorer.h"
#include "CudaScorer.h"
#include "cycleTimer.h"
#include <iostream>

void playGame() {

    //CudaScorer s;
    //OMPScorer s;
    SeqStateScorer s;
    //PthreadStateScorer s;

    Board initial;
    while (true) {
        std::cout << "|0|1|2|3|4|5|6|\n";
        initial.print();
        std::cout << "\n";
        int col;
        std::cin >> col;
        int row = 0;
        while (initial.state[row][col] != 0) {
            row += 1;
        }
        initial.state[row][col] = 1;
        //col = s.searchDFS(initial, -1, 7);
        col = s.searchBFS(initial, -1, 7);
        row = 0;
        while (initial.state[row][col] != 0) {
            row += 1;
        }
        std::cout << "Computer plays: " << col << "\n";
        initial.state[row][col] = -1;
        int result = initial.score();
        if (result == INF) {
            std::cout << "Player Wins!\n";
            std::cout << "|0|1|2|3|4|5|6|\n";
            initial.print();
            break;
        } else if (result == -INF) {
            std::cout << "Computer Wins!\n";
            std::cout << "|0|1|2|3|4|5|6|\n";
            initial.print();
            break;
        }
    }

}



int main (int argc, char *argv[]) {
    OMPScorer o;
    SeqStateScorer s;
    PthreadStateScorer p;
    CudaScorer c;
    //playGame();
    int depth;

    double startSearchTime = 0.f;
    double endSearchTime = 0.f;
    double totalSearchTime = 0.f;

    double startSearchTimeD = 0.f;
    double endSearchTimeD = 0.f;
    double totalSearchTimeD = 0.f;

    double startSearchTimeOB = 0.f;
    double endSearchTimeOB = 0.f;
    double totalSearchTimeOB = 0.f;

    double startSearchTimeOD = 0.f;
    double endSearchTimeOD = 0.f;
    double totalSearchTimeOD = 0.f;

    double startSearchTimeP = 0.f;
    double endSearchTimeP = 0.f;
    double totalSearchTimeP = 0.f;

    double startSearchTimeC = 0.f;
    double endSearchTimeC = 0.f;
    double totalSearchTimeC = 0.f;
    if (argc == 1){
      depth = 7;
      Board initial;

      startSearchTime = CycleTimer::currentSeconds();
      s.searchBFS(initial, 1, depth);
      endSearchTime = CycleTimer::currentSeconds();
      totalSearchTime = endSearchTime - startSearchTime;

      startSearchTimeD = CycleTimer::currentSeconds();
      s.searchDFS(initial, 1, depth);
      endSearchTimeD = CycleTimer::currentSeconds();
      totalSearchTimeD = endSearchTimeD - startSearchTimeD;

      startSearchTimeOB = CycleTimer::currentSeconds();
      o.searchDFS(initial, 1, depth);
      endSearchTimeOB = CycleTimer::currentSeconds();
      totalSearchTimeOB = endSearchTimeOB - startSearchTimeOB;

      startSearchTimeOD = CycleTimer::currentSeconds();
      o.searchDFS(initial, 1, depth);
      endSearchTimeOD = CycleTimer::currentSeconds();
      totalSearchTimeOD = endSearchTimeOD - startSearchTimeOD;

      
      startSearchTimeP = CycleTimer::currentSeconds();
      p.searchDFS(initial, 1, depth);
      endSearchTimeP = CycleTimer::currentSeconds();
      totalSearchTimeP = endSearchTimeP - startSearchTimeP;

      
      startSearchTimeC = CycleTimer::currentSeconds();
      c.searchBFS(initial, 1, depth);
      endSearchTimeC = CycleTimer::currentSeconds();
      totalSearchTimeC = endSearchTimeC - startSearchTimeC;

      printf("Search to a depth of %d\n", depth);
      printf("Sequential (BFS)time taken: %.4f ms\n", 1000*totalSearchTime);
      printf("OMP time (BFS)taken: %.4f ms\n", 1000*totalSearchTimeOB);
      printf("CUDA (BFS) time taken: %.4f ms\n", 1000*totalSearchTimeC);
      printf("Sequential (DFS)time taken: %.4f ms\n", 1000*totalSearchTimeD);
      printf("OMP time (DFS)taken: %.4f ms\n", 1000*totalSearchTimeOD);
      printf("pthread (DFS) time taken: %.4f ms\n----------\n", 1000*totalSearchTimeP);
    }

    for(int i = 1; i < argc; i++){
      Board initial;
      depth = atoi(argv[i]);

      startSearchTime = CycleTimer::currentSeconds();
      s.searchBFS(initial, 1, depth);
      endSearchTime = CycleTimer::currentSeconds();
      totalSearchTime = endSearchTime - startSearchTime;

      startSearchTimeD = CycleTimer::currentSeconds();
      s.searchDFS(initial, 1, depth);
      endSearchTimeD = CycleTimer::currentSeconds();
      totalSearchTimeD = endSearchTimeD - startSearchTimeD;

      startSearchTimeOB = CycleTimer::currentSeconds();
      o.searchDFS(initial, 1, depth);
      endSearchTimeOB = CycleTimer::currentSeconds();
      totalSearchTimeOB = endSearchTimeOB - startSearchTimeOB;

      startSearchTimeOD = CycleTimer::currentSeconds();
      o.searchDFS(initial, 1, depth);
      endSearchTimeOD = CycleTimer::currentSeconds();
      totalSearchTimeOD = endSearchTimeOD - startSearchTimeOD;

      
      startSearchTimeP = CycleTimer::currentSeconds();
      p.searchDFS(initial, 1, depth);
      endSearchTimeP = CycleTimer::currentSeconds();
      totalSearchTimeP = endSearchTimeP - startSearchTimeP;

      
      startSearchTimeC = CycleTimer::currentSeconds();
      c.searchBFS(initial, 1, depth);
      endSearchTimeC = CycleTimer::currentSeconds();
      totalSearchTimeC = endSearchTimeC - startSearchTimeC;

      printf("Search to a depth of %d\n", depth);
      printf("Sequential (BFS)time taken: %.4f ms\n", 1000*totalSearchTime);
      printf("OMP time (BFS)taken: %.4f ms\n", 1000*totalSearchTimeOB);
      printf("CUDA (BFS) time taken: %.4f ms\n", 1000*totalSearchTimeC);
      printf("Sequential (DFS)time taken: %.4f ms\n", 1000*totalSearchTimeD);
      printf("OMP time (DFS)taken: %.4f ms\n", 1000*totalSearchTimeOD);
      printf("pthread (DFS) time taken: %.4f ms\n----------\n", 1000*totalSearchTimeP);
    }
    return 0;
}
