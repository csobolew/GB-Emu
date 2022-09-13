//
// Created by RealL on 9/7/2022.
//

#ifndef GB_EMU_GB_H
#define GB_EMU_GB_H

#include "mmu.h"
#include "cpu.h"
class cpu;

class gb {
private:
    mmu mem;
    friend class mmu;

    cpu processor = cpu(mem);
    friend class cpu;

public:
    void run();
    gb(char* argv[]);
};


#endif //GB_EMU_GB_H
