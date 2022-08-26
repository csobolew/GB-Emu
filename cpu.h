//
// Created by Carson on 8/25/2022.
//

#ifndef GB_EMU_CPU_H
#define GB_EMU_CPU_H
#include <fstream>
#include <iostream>
#include <chrono>
using namespace std;
struct registers {
    //AF, BC, DE, HL pairings
    unsigned char A, B, C, D, E, F, H, L;
};
struct flags {
    //Z - Zero Flag (current operation results in 0)
    //N - Subtract Flag (1 if subtraction was performed)
    //H - Half Carry Flag (1 if carry happened from lower to higher nibble)
    //C - Carry Flag (1 if carry happened in last operation)
    unsigned char Z, N, H, C;
};
class cpu {
private:
     registers reg;
     flags flag;
    unsigned char memory[0xFFFF];
    int pc;
    int sp;
    unsigned char fetch();
    pair<unsigned char, unsigned char> fetch16();
public:
    void load(char* argv[]);
    void step();
};


#endif //GB_EMU_CPU_H
