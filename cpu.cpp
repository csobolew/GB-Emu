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
void cpu::setZ(uint8_t v) {
    if (v) {
        reg.F = reg.F | 0x8;
    }
    else {
        reg.F = reg.F&0x7;
    }
}
uint8_t cpu::getZ() {
    return reg.F>>3;
}
void cpu::setN(uint8_t v) {
    if (v) {
        reg.F = reg.F | 0x4;
    }
    else {
        reg.F = reg.F&0xB;
    }
}
uint8_t cpu::getN() {
    return (reg.F>>2)&0x1;
}
void cpu::setH(uint8_t v) {
    if (v) {
        reg.F = reg.F | 0x2;
    }
    else {
        reg.F = reg.F&0xD;
    }
}
uint8_t cpu::getH() {
    return (reg.F>>1)&0x1;
}
void cpu::setC(uint8_t v) {
    if (v) {
        reg.F = reg.F | 0x1;
    }
    else {
        reg.F = reg.F&0xE;
    }
}
uint8_t cpu::getC() {
    return reg.F&0x1;
}
uint8_t cpu::fetch() {
    uint8_t temp = memory[pc];
    pc++;
    return temp;
}
int8_t cpu::fetchSigned() {
    int8_t temp = memory[pc];
    pc++;
    return temp;
}
void cpu::rotateLeftCarry(uint8_t &reg) {
    uint8_t temp = (reg & 0x80) >> 7;
    reg = reg << 1;
    if (getC() == 1) {
        reg = reg | 0x1;
    }
    setN(0);
    setH(0);
    if (reg == 0) {
        setZ(1);
    }
    setC(temp);
}
uint16_t cpu::fetch16() {
    uint8_t temp1 = fetch();
    uint8_t temp2 = fetch();
    return (((uint16_t)temp2 << 8) | temp1);
}
void cpu::fetch16Reg(uint8_t &reg1, uint8_t &reg2) {
    reg2 = fetch();
    reg1 = fetch();
}
bool cpu::getBit(uint8_t reg, int num) {
    return ((reg >> (num-1))&0x1);
}
void cpu::decrement(uint8_t &reg) {
    setH((((reg) & 0xf) - (1 & 0xf)) & 0x10);
    reg--;
    setN(1);
    if (reg == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
}
void cpu::increment(uint8_t &reg) {
    setH((((reg) & 0xf) + (1 & 0xf)) & 0x10);
    reg++;
    setN(0);
    if (reg == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
}
void cpu::compare(uint8_t reg, uint8_t operand) {
    if (reg == operand) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(1);
   setH(((reg & 0xf) - (operand & 0xf)) & 0x10);
    if (reg < operand) {
        setC(1);
    }
    else {
        setC(0);
    }
}
void cpu::step() {
    switch(fetch()) {
        case 0x00:
            break;
        case 0x01:
            fetch16Reg(reg.B, reg.C);
            break;
        case 0x04:
            increment(reg.B);
            break;
        case 0x05:
            decrement(reg.B);
            break;
        case 0x06:
            reg.B = fetch();
            break;
        case 0x0D:
            decrement(reg.C);
            break;
        case 0x0E:
            reg.C = fetch();
            break;
        case 0x0C:
            increment(reg.C);
            break;
        case 0x11:
            fetch16Reg(reg.D, reg.E);
            break;
        case 0x13:
            reg.DE++;
            break;
        case 0x17: {
            rotateLeftCarry(reg.A);
            break;
        }
        case 0x18:
            pc += fetchSigned();
            break;
        case 0x1A:
            reg.A = memory[reg.DE];
            break;
        case 0x1E:
            reg.E = fetch();
            break;
        case 0x20: {
            int8_t val = fetchSigned();
            if (getZ() == 0) {
                pc += val;
            }
            break;
        }
        case 0x21:
            fetch16Reg(reg.H, reg.L);
            break;
        case 0x22:
            memory[reg.HL] = reg.A;
            reg.HL++;
            break;
        case 0x23:
            reg.HL++;
            break;
        case 0x28: {
            uint8_t val = fetch();
            if (getZ() == 1) {
                pc += val;
            }
            break;
        }
        case 0x2E:
            reg.L = fetch();
            break;
        case 0x31: {
            sp = fetch16();
            break;
        }
        case 0x32:
            memory[reg.HL--] = reg.A;
            break;
        case 0x3D:
            decrement(reg.A);
            break;
        case 0x3E:
            reg.A = fetch();
            break;
        case 0x47:
            reg.B = reg.A;
            break;
        case 0x4F:
            reg.C = reg.A;
            break;
        case 0x57:
            reg.D = reg.A;
            break;
        case 0x67:
            reg.H = reg.A;
            break;
        case 0x77:
            reg.HL = reg.A;
            break;
        case 0x7B:
            reg.A = reg.E;
            break;
        case 0xAF: {
            if ((reg.A ^ reg.A) == 0) {
                setZ(1);
            }
            reg.A = reg.A ^ reg.A;
            break;
        }
        case 0xC1:
            reg.C = memory[sp++];
            reg.B = memory[sp++];
            break;
        case 0xC5:
            sp--;
            memory[sp--] = reg.B;
            memory[sp] = reg.C;
            break;
        case 0xC9: {
            uint8_t temp1 = memory[sp++];
            uint8_t temp2 = memory[sp++];
            pc = (((uint16_t) temp2 << 8) | temp1);
            break;
        }
        //CB-prefix codes
        case 0xCB:
            switch(fetch()) {
                case 0x11: {
                    rotateLeftCarry(reg.C);
                    break;
                }
                case 0x7C:
                    setZ(!getBit(reg.H, 7));
                    setN(0);
                    setH(1);
                    break;
                default:
                    cout << "Unknown Opcode " << hex << (unsigned int)memory[pc-1] << " at 0x" << pc-1 << endl;
                    exit(1);
            }
            break;
        case 0xCD: {
            uint16_t temp = fetch16();
            sp--;
            memory[sp--] = pc >> 8;
            memory[sp] = pc&0x00FF;
            pc = temp;
            break;
            }
        case 0xE0:
            memory[0xFF00+fetch()] = reg.A;
            break;
        case 0xE2:
            memory[0xFF00+reg.C] = reg.A;
            break;
        case 0xEA:
            memory[fetch16()] = reg.A;
            break;
        case 0xF0:
            reg.A = memory[0xFF00+fetch()];
            break;
        case 0xFE: {
            compare(reg.A, fetch());
            break;
        }
        default:
            cout << "Unknown Opcode " << hex << (unsigned int)memory[pc-1] << " at 0x" << pc-1 << endl;
            exit(1);
    }
}

cpu::cpu() {}

registers::registers() {}
