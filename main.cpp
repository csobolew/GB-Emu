//
// Created by Carson on 8/25/2022.
//
#include "gb.h"
int main(int argc, char* argv[]) {
    gb gameboy = gb(argv);
    gameboy.run();
    cout << "test" << endl;
    return 0;
}