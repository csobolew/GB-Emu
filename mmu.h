//
// Created by RealL on 9/8/2022.
//

#ifndef GB_EMU_MMU_H
#define GB_EMU_MMU_H
#include <cstdint>
#include <fstream>
#include <iostream>
using namespace std;

class mmu {
private:
    uint8_t memory[0xFFFF] = {};
public:
    uint8_t readMem(uint16_t code);
    void writeMem(uint16_t addr, uint8_t val);
    void load(char* argv[]);

};


#endif //GB_EMU_MMU_H
