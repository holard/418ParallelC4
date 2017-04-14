
#include "SeqStateScorer.h"
#include <iostream>

// TODO: Implement harness here. You may want to initialize different boards to see how
//       different implementations perform in different situations.

int main () {
    SeqStateScorer s;

    Board initial;
    initial.print();
    std::cout << "\n\n";
    //std::cout << "initial[5][5] = " << initial.state[5][5] << "\n";
    
    // insert timing code
    s.searchToDepth(initial, 0, 0);
    // timing code
    // print result
    return 0;
}
