//
// Created by RealL on 9/14/2022.
//

#include "timer.h"
void timer::updateFreq() {
    switch(mem->readMem(0xFF07)&0b11) {
        case 0b00:
            frequency = 4096;
            timerCounter = clockspeed / frequency;
            break;
        case 0b01:
            frequency = 262144;
            timerCounter = clockspeed / frequency;
            break;
        case 0b10:
            frequency = 65536;
            timerCounter = clockspeed / frequency;
            break;
        case 0b11:
            frequency = 16384;
            timerCounter = clockspeed / frequency;
            break;
    }
}
void timer::updateTimer(int cycles) {
    //Do divider
    if ((mem->readMem(0xFF07) & 0b100) == 0b100) { //Timer enabled
        timerCounter -= cycles;
        if (timerCounter <= 0) {
            updateFreq();

            if (mem->readMem(0xFF05) == 255) {
                mem->writeMem(0xFF05, mem->readMem(0xFF06)); //Reset counter
                mem->writeMem(0xFF0F, mem->readMem(0xFF0F) | 0b00100); //Request interrupt
            }
            else {
                mem->writeMem(0xFF05, mem->readMem(0xFF05)+1);
            }
        }
    }
}
timer::timer(mmu& memory) : mem(&memory) {

}