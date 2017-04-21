
#include "SeqStateScorer.h"
#include "cycleTimer.h"
#include <iostream>

void playGame() {

    SeqStateScorer s;

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


// TODO: Implement harness here. You may want to initialize different boards to see how
//       different implementations perform in different situations.

int main () {
    SeqStateScorer s;
    double startSearchTime = 0.f;
    double endSearchTime = 0.f;
    double totalSearchTime = 0.f;

    Board initial;
    //std::cout << "initial[5][5] = " << initial.state[5][5] << "\n";
    //playGame();
    //
    startSearchTime = CycleTimer::currentSeconds();

    s.searchToDepth(initial, 1, 6);

    endSearchTime = CycleTimer::currentSeconds();
    totalSearchTime = endSearchTime - startSearchTime;

    // print result
    printf("Time taken: %.4f ms\n", 1000*totalSearchTime);
    return 0;
}
