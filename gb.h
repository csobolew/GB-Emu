//
// Created by RealL on 9/7/2022.
//

#ifndef GB_EMU_GB_H
#define GB_EMU_GB_H


#include "cpu.h"
#include "mmu.h"
struct registers {
    registers();

//AF, BC, DE, HL pairings
    union {
        struct {
            uint8_t F = 0xB0;
            uint8_t A = 0x01;
        };
        uint16_t AF;
    };
    union {
        struct {
            uint8_t C = 0x13;
            uint8_t B = 0;
        };
        uint16_t BC;
    };
    union {
        struct {
            uint8_t E = 0xD8;
            uint8_t D = 0;
        };
        uint16_t DE;
    };
    union {
        struct {
            uint8_t L = 0x4D;
            uint8_t H = 0x01;
        };
        uint16_t HL;
    };
    union {
        struct {
            uint8_t P = 0xFE;
            uint8_t S = 0xFF;
        };
        uint16_t SP;
    };
};
class gb {
private:
    registers reg;

    cpu processor;
    friend class cpu;

    mmu mem;
    friend class mmu;

    uint8_t memory[0xFFFF] = {};
    int pc = 0x100;
    uint8_t IME = 0;
    void load(char* argv[]);
public:
    void run();
    gb(char* argv[]);
};


#endif //GB_EMU_GB_H
