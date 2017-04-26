#include "OMPScorer.h"
#include "SeqStateScorer.h"
#include "cycleTimer.h"
#include <iostream>

void playGame() {

    OMPScorer s;
    //SeqStateScorer s;

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
        col = s.searchToDepth(initial, -1, 7);
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


// TODO: Initialize different boards to see how
//       different implementations perform in different situations.

int main (int argc, char *argv[]) {
    OMPScorer o;
    SeqStateScorer s;

    double startSearchTime = 0.f;
    double endSearchTime = 0.f;
    double totalSearchTime = 0.f;

    double startSearchTimeO = 0.f;
    double endSearchTimeO = 0.f;
    double totalSearchTimeO = 0.f;
    int depth;

    if (argc == 1){
      depth = 8;
      Board initial;
      startSearchTime = CycleTimer::currentSeconds();

      s.searchToDepth(initial, 1, depth);

      endSearchTime = CycleTimer::currentSeconds();
      totalSearchTime = endSearchTime - startSearchTime;

      startSearchTimeO = CycleTimer::currentSeconds();

      o.searchToDepth(initial, 1, depth);

      endSearchTimeO = CycleTimer::currentSeconds();
      totalSearchTimeO = endSearchTimeO - startSearchTimeO;

      printf("Search to a depth of %d\n", depth);
      printf("Sequential time taken: %.4f ms\n", 1000*totalSearchTime);
      printf("OMP time taken: %.4f ms\n----------\n", 1000*totalSearchTimeO);
    }

    for(int i = 1; i < argc; i++){
      Board initial;
      startSearchTime = 0.f;
      endSearchTime = 0.f;
      totalSearchTime = 0.f;

      startSearchTimeO = 0.f;
      endSearchTimeO = 0.f;
      totalSearchTimeO = 0.f;
      depth = atoi(argv[i]);

      startSearchTime = CycleTimer::currentSeconds();

      s.searchToDepth(initial, 1, depth);

      endSearchTime = CycleTimer::currentSeconds();
      totalSearchTime = endSearchTime - startSearchTime;

      startSearchTimeO = CycleTimer::currentSeconds();

      o.searchToDepth(initial, 1, depth);

      endSearchTimeO = CycleTimer::currentSeconds();
      totalSearchTimeO = endSearchTimeO - startSearchTimeO;

      printf("Search to a depth of %d\n", depth);
      printf("Sequential time taken: %.4f ms\n", 1000*totalSearchTime);
      printf("OMP time taken: %.4f ms\n----------\n", 1000*totalSearchTimeO);
    }
    return 0;
}
