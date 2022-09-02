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
    memory[0xFF44] = 0x90;
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

uint8_t cpu::getLower(uint16_t val) {
    return val&0xFF;
}

uint8_t cpu::getUpper(uint16_t val) {
    return val >> 8;
}
void cpu::op_LDu16SP() {
    uint16_t val = fetch16();
    memory[val] = reg.P;
    memory[val+1] = reg.S;
}
void cpu::op_jumpRel() {
    pc += fetchSigned();
}
bool cpu::getCondition(uint8_t code) {
    uint8_t temp = (code&0x18) >> 3;
    switch (temp) {
        case 0x00:
            if (getZ() == 0) {
                return true;
            }
            break;
        case 0x01:
            if (getZ() == 1) {
                return true;
            }
        case 0x02:
            if (getC() == 0) {
                return true;
            }
            break;
        case 0x03:
            if (getC() == 1) {
                return true;
            }
            break;
    }
    return false;
}
void cpu::op_jumpRelCond(uint8_t val) {
    if (getCondition(val)) {
        op_jumpRel();
    }
}
uint8_t& cpu::get1R16L(uint8_t val) {
    switch((val&0x30) >> 4) {
        case 0x0:
            return reg.C;
            break;
        case 0x1:
            return reg.E;
            break;
        case 0x2:
            return reg.L;
            break;
        case 0x3:
            return reg.P;
            break;
            }
    }
uint8_t& cpu::get1R16U(uint8_t val) {
    switch((val&0x30) >> 4) {
        case 0x0:
            return reg.B;
            break;
        case 0x1:
            return reg.D;
            break;
        case 0x2:
            return reg.H;
            break;
        case 0x3:
            return reg.S;
            break;
    }
}
uint16_t& cpu::get1R16(uint8_t val) {
    switch((val&0b00110000) >> 4) {
        case 0b00:
            return reg.BC;
            break;
        case 0b01:
            return reg.DE;
            break;
        case 0b10:
            return reg.HL;
            break;
        case 0b11:
            return reg.SP;
            break;
    }
}
void cpu::op_ldr16u16(uint8_t val) {
    get1R16L(val) = fetch();
    get1R16U(val) = fetch();
}
void cpu::op_addHLr16(uint8_t val) {
    reg.HL += get1R16(val);
}
void cpu::step() {
    uint8_t val = fetch();
    switch((val&0b11000000) >> 6) { //First 2
        case 0x00:
            switch(val&0b00000111) { //Last 3
                case 0b000:
                    switch((val&0b00111000) >> 3) { //Middle 3
                        case 0b000: //NOP
                            break;
                        case 0b001: //LD (u16), SP
                            op_LDu16SP();
                            break;
                        case 0b010: //STOP
                            exit(1);
                            break;
                        case 0b011: //JR (unconditional)
                            op_jumpRel();
                            break;
                        default: //JR (conditional)
                            op_jumpRelCond(val);
                            break;
                    }
                    break;
                case 0b001:
                    switch((val&0b00001000) >> 3) {
                        case 0b0: //LD r16 (group 1), u16
                            op_ldr16u16(val);
                            break;
                        case 0b1: //ADD HL, r16
                            op_addHLr16(val);
                            break;
                    }
                    break;
                case 0b010:

                    break;
            }
            break;
    }
}

cpu::cpu() {}

registers::registers() {}
