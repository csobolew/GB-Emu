//
// Created by RealL on 9/14/2022.
//

#ifndef GB_EMU_TIMER_H
#define GB_EMU_TIMER_H
#include "mmu.h"

class timer {
private:
    int clockspeed = 4194304;
    int frequency = 4096;
    mmu* mem;
    int timerCounter = clockspeed/frequency;
    void updateFreq();
public:
    void updateTimer(int cycles);
    timer(mmu& memory);
};


#endif //GB_EMU_TIMER_H
