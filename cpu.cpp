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
    if (v == 1) {
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
    if (v == 1) {
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
    if (v == 1) {
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
    if (v == 1) {
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
uint16_t& cpu::get2R16(uint8_t val) {
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
            return reg.HL;
            break;
    }
}
uint8_t& cpu::getR8(uint8_t val) {
    switch((val&0b00111000) >> 3) {
        case 0b000:
            return reg.B;
            break;
        case 0b001:
            return reg.C;
            break;
        case 0b010:
            return reg.D;
            break;
        case 0b011:
            return reg.E;
            break;
        case 0b100:
            return reg.H;
            break;
        case 0b101:
            return reg.L;
            break;
        case 0b110:
            return memory[reg.HL];
            break;
        case 0b111:
            return reg.A;
            break;
    }
}
uint8_t& cpu::getR8L(uint8_t val) {
    switch(val&0b00000111) {
        case 0b000:
            return reg.B;
            break;
        case 0b001:
            return reg.C;
            break;
        case 0b010:
            return reg.D;
            break;
        case 0b011:
            return reg.E;
            break;
        case 0b100:
            return reg.H;
            break;
        case 0b101:
            return reg.L;
            break;
        case 0b110:
            return memory[reg.HL];
            break;
        case 0b111:
            return reg.A;
            break;
    }
}
void cpu::op_ldr16u16(uint8_t val) {
    get1R16L(val) = fetch();
    get1R16U(val) = fetch();
}
void cpu::op_addHLr16(uint8_t val) {
    setH(((reg.HL & 0xFFF) + (get1R16(val) & 0xFFF)) > 0xFFF);
    if ((reg.HL + get1R16(val)) > 0xFFFF) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.HL += get1R16(val);
    setN(0);

}
void cpu::op_ldr16A(uint8_t val) {
    memory[get2R16(val)] = reg.A;
    if (((val&0b00110000) >> 4) == 0b11) {
        reg.HL--;
    }
}
void cpu::op_ldAr16(uint8_t val) {
    reg.A = memory[get2R16(val)];
    if (((val&0b00110000) >> 4) == 0b11) {
        reg.HL--;
    }
}
void cpu::op_INCr16(uint8_t val) {
    get1R16(val)++;
}
void cpu::op_DECr16(uint8_t val) {
    get1R16(val)--;
}
void cpu::op_INCr8(uint8_t val) {
    setH(((getR8(val) & 0xF) + (1 & 0xF)) > 0xF);
    getR8(val)++;
    setN(0);
    if (getR8(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
}
void cpu::op_DECr8(uint8_t val) {
    setH(((getR8(val) & 0xf) - (1 & 0xf)) < 0);
    getR8(val)--;
    setN(1);
    if (getR8(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
}
void cpu::op_ldr8u8(uint8_t val) {
    getR8(val) = fetch();
}
void cpu::op_ldr8r8(uint8_t val) {
    getR8(val) = getR8L(val);
}
void cpu::op_group1(uint8_t val) {
    switch((val&0b00111000) >> 3) {
        case 0b000: //RLCA
            op_rlca();
            break;
        case 0b001: //RRCA
            op_rrca();
            break;
        case 0b010: //RLA
            op_rla();
            break;
        case 0b011: //RRA
            op_rra();
            break;
        case 0b100: //DAA
            op_daa();
            break;
        case 0b101: //CPL
            op_cpl();
            break;
        case 0b110: //SCF
            op_scf();
            break;
        case 0b111: //CCF
            op_ccf();
            break;
    }
}
void cpu::op_ccf() {
    setN(0);
    setH(0);
    if (getC() == 0) {
        setC(1);
    }
    else if (getC() == 1) {
        setC(0);
    }
}
void cpu::op_scf() {
    setN(0);
    setH(0);
    setC(1);
}
void cpu::op_cpl() {
    reg.A = ~reg.A;
    setN(1);
    setH(1);
}
void cpu::op_daa() {
    if (getN() == 0) {
        if ((getC() == 1) || (reg.A > 0x99)) {
            reg.A += 0x60;
            setC(1);
        }
        if ((getH() == 1) || ((reg.A&0x0F) > 0x09)) {
            reg.A += 0x60;
        }
    }
    else {
        if (getC() == 1) {
            reg.A -= 0x60;
        }
        if (getH() == 1) {
            reg.A -= 0x60;
        }
    }
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setH(0);
}
void cpu::op_rrca() {
    uint8_t temp = (reg.A & 0x1);
    reg.A = reg.A >> 1;
    if (temp == 1) {
        reg.A = reg.A | 0b10000000;
    }
    setN(0);
    setH(0);
    setZ(0);
    setC(temp);
}
void cpu::op_rra() {
    uint8_t temp = (reg.A & 0x1);
    reg.A = reg.A >> 1;
    if (getC() == 1) {
        reg.A = reg.A | 0b10000000;
    }
    setN(0);
    setH(0);
    setZ(0);
    setC(temp);
}
void cpu::op_rlca() {
    uint8_t temp = (reg.A & 0x80) >> 7;
    reg.A = reg.A << 1;
    if (temp == 1) {
        reg.A = reg.A | 0x1;
    }
    setN(0);
    setH(0);
    setZ(0);
    setC(temp);
}
void cpu::op_rla() {
    uint8_t temp = (reg.A & 0x80) >> 7;
    reg.A = reg.A << 1;
    if (getC() == 1) {
        reg.A = reg.A | 0x1;
    }
    setN(0);
    setH(0);
    setZ(0);
    setC(temp);
}
void cpu::op_aluar8(uint8_t val) {
    switch((val&0b00111000) >> 3) {
        case 0b000: //ADD
            op_aluaddA(val);
            break;
        case 0b001: //ADC
            op_aluadcA(val);
            break;
        case 0b010: //SUB
            op_alusubA(val);
            break;
        case 0b011: //SBC
            op_alusbcA(val);
            break;
        case 0b100: //AND
            op_aluandA(val);
            break;
        case 0b101: //XOR
            op_aluxorA(val);
            break;
        case 0b110: //OR
            op_aluorA(val);
            break;
        case 0b111: //CP
            op_alucpA(val);
            break;
    }
}
void cpu::op_aluaddA(uint8_t val) {
    reg.A += getR8L(val);
}
void cpu::op_aluadcA(uint8_t val) {

}
void cpu::op_alusubA(uint8_t val) {

}
void cpu::op_alusbcA(uint8_t val) {

}
void cpu::op_aluandA(uint8_t val) {

}
void cpu::op_aluxorA(uint8_t val) {

}
void cpu::op_aluorA(uint8_t val) {

}
void cpu::op_alucpA(uint8_t val) {

}
void cpu::step() {
    uint8_t val = fetch();
    switch((val&0b11000000) >> 6) { //First 2
        case 0b00:
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
                        case 0b1: //ADD HL, r16 -- Flags
                            op_addHLr16(val);
                            break;
                    }
                    break;
                case 0b010:
                    switch((val&0b00001000) >> 3) {
                        case 0b0: //LD (r16), A (group 2)
                            op_ldr16A(val);
                            break;
                        case 0b1: //LD A,(r16) (group 2)
                            op_ldAr16(val);
                            break;
                    }
                    break;
                case 0b011:
                    switch((val&0b00001000) >> 3) {
                        case 0b0: //INC r16
                            op_INCr16(val);
                            break;
                        case 0b1: //DEC r16
                            op_DECr16(val);
                            break;
                    }
                    break;
                case 0b100: //INC r8
                    op_INCr8(val);
                    break;
                case 0b101: //DEC r8
                    op_DECr8(val);
                    break;
                case 0b110: //LD r8, u8
                    op_ldr8u8(val);
                    break;
                case 0b111: //opcode group 1
                    op_group1(val);
                    break;
            }
            break;
        case 0b01:
            switch(val&0b00111111) {
                case 0b110110: //HALT - Not implemented
                    exit(5);
                    break;
                default: //LD r8, r8
                    op_ldr8r8(val);
                    break;
            }
            break;
        case 0b10: //ALU A, r8
            op_aluar8(val);
            break;
    }
}

cpu::cpu() {}

registers::registers() {}
