//
// Created by RealL on 9/7/2022.
//

#ifndef GB_EMU_GB_H
#define GB_EMU_GB_H

#include "mmu.h"
#include "cpu.h"
#include "timer.h"
class cpu;

class gb {
private:
    mmu mem;
    int cycles;
    cpu processor = cpu(mem);
    timer tim = timer(mem);

public:
    void run();
    gb(char* argv[]);
};


#endif //GB_EMU_GB_H
