//
// Created by Carson on 8/25/2022.
//
#include "cpu.h"
int main(int argc, char* argv[]) {
    cpu game = cpu();
    game.load(argv);
    game.step();
    cout << "test" << endl;
    return 0;
}