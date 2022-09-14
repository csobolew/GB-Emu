//
// Created by RealL on 9/14/2022.
//

#ifndef GB_EMU_TIMER_H
#define GB_EMU_TIMER_H
#include "mmu.h"

class timer {
private:
    int clockspeed = 4194304;
    mmu* mem;
    int timerCounter = clockspeed / 4096;
    int dividerCounter = 0;
public:
    void updateTimer(int cycles);
    timer(mmu& memory);
};


#endif //GB_EMU_TIMER_H
