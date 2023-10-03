#include "Sulakarta.h"

int main() {
    SingelThreadAlphaBetaPro::init();
    MCTS::init();
    MCTS::work(0);
    return 0;
}