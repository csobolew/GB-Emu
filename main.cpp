//
// Created by Carson on 8/25/2022.
//
#include "gb.h"
int main(int argc, char* argv[]) {
    ofstream outfile("log.txt");
    gb game = gb(argv);
    game.run();
    cout << "test" << endl;
    return 0;
}