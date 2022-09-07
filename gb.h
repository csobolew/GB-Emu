//
// Created by RealL on 9/7/2022.
//

#ifndef GB_EMU_GB_H
#define GB_EMU_GB_H


#include "cpu.h"

class gb {
private:
    cpu processor;
    uint8_t memory[0xFFFF] = {};
    void load(char* argv[]);
public:
    void run();
    gb(char* argv[]);
};


#endif //GB_EMU_GB_H
