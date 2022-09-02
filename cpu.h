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
    registers();

//AF, BC, DE, HL pairings
    union {
        struct {
            uint8_t F = 0;
            uint8_t A = 0;
        };
        uint16_t AF;
    };
    union {
        struct {
            uint8_t C = 0;
            uint8_t B = 0;
        };
        uint16_t BC;
    };
    union {
        struct {
            uint8_t E = 0;
            uint8_t D = 0;
        };
        uint16_t DE;
    };
    union {
        struct {
            uint8_t L = 0;
            uint8_t H = 0;
        };
        uint16_t HL;
    };
    union {
        struct {
            uint8_t P = 0;
            uint8_t S = 0;
        };
        uint16_t SP;
    };
};

class cpu {
private:
    registers reg;
    uint8_t memory[0xFFFF] = {};
    int pc = 0;
    uint8_t fetch();
    uint16_t fetch16();
    void fetch16Reg(uint8_t &reg1, uint8_t &reg2);
    bool getBit(uint8_t reg, int num);
    void setZ(uint8_t v);
    uint8_t getZ();
    void setN(uint8_t v);
    uint8_t getN();
    void setH(uint8_t v);
    uint8_t getH();
    void setC(uint8_t v);
    uint8_t getC();
    void decrement(uint8_t &reg);
    void increment(uint8_t &reg);
    void rotateLeftCarry(uint8_t &reg);
    int8_t fetchSigned();
    uint8_t getLower(uint16_t val);
    uint8_t getUpper(uint16_t val);
    void compare(uint8_t reg, uint8_t operand);

    void op_jumpRel();
    void op_ldr16u16(uint8_t code);
    uint8_t& get1R16L(uint8_t code);
    uint8_t& get1R16U(uint8_t code);
    uint16_t& get1R16(uint8_t code);
    void op_jumpRelCond(uint8_t code);
    void op_addHLr16(uint8_t code);
    void op_LDu16SP ();
    bool getCondition(uint8_t code);
public:
    void load(char* argv[]);
    void step();
    cpu();
};


#endif //GB_EMU_CPU_H
