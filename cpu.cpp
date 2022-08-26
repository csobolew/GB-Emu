//
// Created by Carson on 8/25/2022.
//

#include "cpu.h"

void cpu::load(char* argv[]) {
    FILE* f = fopen("gbboot.gb", "rb");
    if (f == nullptr) {
        cout << "Couldn't open file";
        exit(1);
    }
    int i = 0;
    while (fread(&memory[i], 1, 1, f)) {
        i++;
    }
    fclose(f);
}

uint8_t cpu::fetch() {
    uint8_t temp = memory[pc];
    pc++;
    return temp;
}
uint16_t cpu::fetch16() {
    uint8_t temp1 = fetch();
    uint8_t temp2 = fetch();
    return (((uint16_t)temp2 << 8) | temp1);
}
bool cpu::getBit(uint8_t reg, int num) {
    return ((reg >> (num-1))&0x1);
}
void cpu::setFlags(uint16_t Z, uint16_t N, uint16_t H, uint16_t C) {
    if (Z != 0xFFFF) {
        flag.Z = Z;
    }
    if (N != 0xFFFF) {
        flag.N = N;
    }
    if (H != 0xFFFF) {
        flag.H = H;
    }
    if (C != 0xFFFF) {
        flag.C = C;
    }
}
void cpu::step() {
    switch(fetch()) {

        case 0x00:
            break;
        case 0x01:
            reg.BC = fetch16();
            break;
        case 0x0E:
            reg.C = fetch();
            break;
        case 0x20:
            if (flag.Z == 0) {
                pc += fetch();
            }
            break;
        case 0x0C:
            reg.C++;
            break;
        case 0x11:
            reg.DE = fetch16();
            break;
        case 0x1A:
            reg.A = reg.DE;
            break;
        case 0x21:
            reg.HL = fetch16();
            break;
        case 0x31: {
            sp = fetch16();
            break;
        }
        case 0x32:
            reg.HL = reg.A;
            reg.HL--;
            break;
        case 0x3E:
            reg.A = fetch();
            break;
        case 0x47:
            reg.B = reg.A;
            break;
        case 0x77:
            reg.HL = reg.A;
            break;
        case 0xAF: {
            if ((reg.A ^ reg.A) == 0) {
                flag.Z = 1;
            }
            reg.A = reg.A ^ reg.A;
            break;
        }


        //CB-prefix codes
        case 0xCB:
            switch(fetch()) {
                case 0x7C:
                    setFlags(!getBit(reg.H, 7), 0, 1, 0xFFFF);
                    break;
            }
            break;
        case 0xE0:
            memory[0xFF00+fetch()] = reg.A;
            break;
        case 0xE2:
            memory[0xFF00+reg.C] = reg.A;
            break;
        default:
            cout << "Unknown Opcode " << memory[pc-1] << " at " << pc-1 << endl;
            exit(1);
    }
}