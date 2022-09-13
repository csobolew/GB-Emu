//
// Created by RealL on 9/8/2022.
//

#include "mmu.h"
uint8_t mmu::readMem(uint16_t addr) {
    return memory[addr];
};
void mmu::writeMem(uint16_t addr, uint8_t val) {
    memory[addr] = val;
}
void mmu::load(char* argv[]) {
    FILE* f = fopen("gbb.gb", "rb");
    if (f == nullptr) {
        cout << "Couldn't open file";
        exit(1);
    }
    int i = 0;
    while (fread(&memory[i], 1, 1, f)) {
        i++;
    }
    fclose(f);
    memory[0xFF44] = 0x90;
}