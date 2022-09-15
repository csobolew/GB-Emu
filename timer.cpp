//
// Created by RealL on 9/14/2022.
//

#include "timer.h"
void timer::updateTimer(int cycles) {
    if ((mem->readMem(0xFF07) & 0b100) == 0b100) { //Timer enabled
        timerCounter -= cycles;
        while (timerCounter <= 0) {
            int frequency = 4096;

            if ((mem->readMem(0xFF07) & 0b11) == 0b01) {
                frequency = 262144;
            }
            else if ((mem->readMem(0xFF07) & 0b11) == 0b10) {
                frequency = 65536;
            }
            else if ((mem->readMem(0xFF07) & 0b11) == 0b11) {
                frequency = 16384;
            }
            if (mem->readMem(0xFF05) == 0xFF) {
                mem->writeMem(0xFF05, mem->readMem(0xFF06)); //Reset counter
                mem->writeMem(0xFF0F, mem->readMem(0xFF0F) | 0b00100); //Request interrupt
            }
            else {
                mem->writeMem(0xFF05, mem->readMem(0xFF05) + 1);
            }
            timerCounter += clockspeed/frequency;
        }
    }
}


timer::timer(mmu& memory) : mem(&memory) {

}