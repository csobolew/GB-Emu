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
    union {
        struct {
            uint8_t A;
            uint8_t F;
        };
        uint16_t AF;
    };
    union {
        struct {
            uint8_t B;
            uint8_t C;
        };
        uint16_t BC;
    };
    union {
        struct {
            uint8_t D;
            uint8_t E;
        };
        uint16_t DE;
    };
    union {
        struct {
            uint8_t H;
            uint8_t L;
        };
        uint16_t HL;
    };
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
    uint8_t memory[0xFFFF];
    int pc;
    int sp;
    uint8_t fetch();
    uint16_t fetch16();
    bool getBit(uint8_t reg, int num);
    void setFlags(uint16_t Z, uint16_t N, uint16_t H, uint16_t C);
public:
    void load(char* argv[]);
    void step();
};


#endif //GB_EMU_CPU_H
