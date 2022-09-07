//
// Created by Carson on 8/25/2022.
//

#include "cpu.h"
#include <iomanip>

void cpu::log(ofstream& outfile) {
    outfile << "A: ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)reg.A;
    outfile << " F: ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)reg.F;
    outfile << " B: ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)reg.B;
    outfile << " C: ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)reg.C;
    outfile << " D: ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)reg.D;
    outfile << " E: ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)reg.E;
    outfile << " H: ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)reg.H;
    outfile << " L: ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)reg.L;
    outfile << " SP: ";
    outfile << setfill('0') << setw(4) << right << hex << uppercase << (int)reg.SP;
    outfile << " PC: 00:";
    outfile << setfill('0') << setw(4) << right << hex << uppercase << pc;
    outfile << " (";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)memory[pc];
    outfile << " ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)memory[pc+1];
    outfile << " ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)memory[pc+2];
    outfile << " ";
    outfile << setfill('0') << setw(2) << right << hex << uppercase << (int)memory[pc+3];
    outfile << ")" << endl;
}
void cpu::setZ(uint8_t v) {
    if (v == 1) {
        reg.F = reg.F | 0b10000000;
    }
    else {
        reg.F = reg.F&0b01111111;
    }
}
uint8_t cpu::getZ() const {
    return reg.F>>7;
}
void cpu::setN(uint8_t v) {
    if (v == 1) {
        reg.F = reg.F | 0b01000000;
    }
    else {
        reg.F = reg.F&0b10111111;
    }
}
uint8_t cpu::getN() const {
    return (reg.F>>6)&0x1;
}
void cpu::setH(uint8_t v) {
    if (v == 1) {
        reg.F = reg.F | 0b00100000;
    }
    else {
        reg.F = reg.F&0b11011111;
    }
}
uint8_t cpu::getH() const {
    return (reg.F>>5)&0x1;
}
void cpu::setC(uint8_t v) {
    if (v == 1) {
        reg.F = reg.F | 0b00010000;
    }
    else {
        reg.F = reg.F&0b11101111;
    }
}

uint8_t cpu::getC() const {
    return (reg.F>>4)&0x1;
}
uint8_t cpu::fetch() {
    uint8_t temp = memory[pc];
    pc++;
    return temp;
}
int8_t cpu::fetchSigned() {
    auto temp = (int8_t)memory[pc];
    pc++;
    return temp;
}

uint16_t cpu::fetch16() {
    uint8_t temp1 = fetch();
    uint8_t temp2 = fetch();
    return (((uint16_t)temp2 << 8) | temp1);
}
bool cpu::getBit(uint8_t regi, int num) {
    return ((regi >> (num))&0x1);
}
void cpu::op_LDu16SP() {
    uint16_t val = fetch16();
    memory[val] = reg.P;
    memory[val+1] = reg.S;
}
void cpu::op_jumpRel(int8_t val) {
    pc += val;
}
bool cpu::getCondition(uint8_t code) {
    uint8_t temp = (code&0b00011000) >> 3;
    switch (temp) {
        case 0b00:
            if (getZ() == 0) {
                return true;
            }
            break;
        case 0b01: {
            if (getZ() == 1) {
                return true;
            }
            break;
        }
        case 0b10: {
            if (getC() == 0) {
                return true;
            }
            break;
        }
        case 0b11:
            if (getC() == 1) {
                return true;
            }
            break;
        default:
            cout << "Unknown condition" << endl;
            exit(15);
    }
    return false;
}
void cpu::op_jumpRelCond(uint8_t val) {
    int8_t temp = fetchSigned();
    if (getCondition(val)) {
        op_jumpRel(temp);
    }
}
uint8_t& cpu::get1R16L(uint8_t val) {
    switch((val&0b00110000) >> 4) {
        case 0b00:
            return reg.C;
        case 0b01:
            return reg.E;
        case 0b10:
            return reg.L;
        case 0b11:
            return reg.P;
        default:
            cout << "Unknown r16" << endl;
            exit(20);
            }
    }
uint8_t& cpu::get1R16U(uint8_t val) {
    switch((val&0b00110000) >> 4) {
        case 0b00:
            return reg.B;
        case 0b01:
            return reg.D;
        case 0b10:
            return reg.H;
        case 0b11:
            return reg.S;
        default:
            cout << "Unknown r16" << endl;
            exit(21);
    }
}
uint16_t& cpu::get1R16(uint8_t val) {
    switch((val&0b00110000) >> 4) {
        case 0b00:
            return reg.BC;
        case 0b01:
            return reg.DE;
        case 0b10:
            return reg.HL;
        case 0b11:
            return reg.SP;
        default:
            cout << "Unknown r16" << endl;
            exit(22);
    }
}
uint16_t& cpu::get2R16(uint8_t val) {
    switch((val&0b00110000) >> 4) {
        case 0b00:
            return reg.BC;
        case 0b01:
            return reg.DE;
        case 0b10 ... 0b11:
            return reg.HL;
        default:
            cout << "Unknown r16" << endl;
            exit(23);
    }
}
uint8_t& cpu::getR8(uint8_t val) {
    switch((val&0b00111000) >> 3) {
        case 0b000:
            return reg.B;
        case 0b001:
            return reg.C;
        case 0b010:
            return reg.D;
        case 0b011:
            return reg.E;
        case 0b100:
            return reg.H;
        case 0b101:
            return reg.L;
        case 0b110:
            return memory[reg.HL];
        case 0b111:
            return reg.A;
        default:
            cout << "Unknown r8" << endl;
            exit(24);
    }
}
uint8_t& cpu::getR8L(uint8_t val) {
    switch(val&0b00000111) {
        case 0b000:
            return reg.B;
        case 0b001:
            return reg.C;
        case 0b010:
            return reg.D;
        case 0b011:
            return reg.E;
        case 0b100:
            return reg.H;
        case 0b101:
            return reg.L;
        case 0b110:
            return memory[reg.HL];
        case 0b111:
            return reg.A;
        default:
            cout << "Unknown r8" << endl;
            exit(25);
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
    if (((val&0b00110000) >> 4) == 0b10) {
        reg.HL++;
    }
    else if (((val&0b00110000) >> 4) == 0b11) {
        reg.HL--;
    }
}
void cpu::op_ldAr16(uint8_t val) {
    reg.A = memory[get2R16(val)];
    if (((val&0b00110000) >> 4) == 0b10) {
        reg.HL++;
    }
    else if (((val&0b00110000) >> 4) == 0b11) {
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
            reg.A += 0x6;
        }
    }
    else {
        if (getC() == 1) {
            reg.A -= 0x60;
        }
        if (getH() == 1) {
            reg.A -= 0x6;
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
    setH(((reg.A & 0xf) + (getR8L(val) & 0xf)) > 0xf);
    if ((reg.A + getR8L(val)) > 0xFF) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.A += getR8L(val);
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
}
void cpu::op_aluadcA(uint8_t val) {
    setH(((reg.A & 0xf) + (getR8L(val) & 0xf) + getC()) > 0xf);
    uint8_t temp = getC();
    if ((reg.A + getR8L(val) + getC()) > 0xFF) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.A += getR8L(val) + temp;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
}
void cpu::op_alusubA(uint8_t val) {
    setH(((reg.A & 0xf) - (getR8L(val) & 0xf)) & 0x10);
    if (reg.A < getR8L(val)) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.A -= getR8L(val);
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(1);
}
void cpu::op_alusbcA(uint8_t val) {
    setH(((reg.A & 0xf) - (getR8L(val) & 0xf) - getC()) & 0x10);
    uint8_t temp = getC();
    if (reg.A < (getR8L(val) + getC())) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.A -= getR8L(val) + temp;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(1);
}
void cpu::op_aluandA(uint8_t val) {
    reg.A = reg.A&getR8L(val);
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
    setH(1);
    setC(0);
}
void cpu::op_aluxorA(uint8_t val) {
    reg.A = reg.A ^ getR8L(val);
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
    setH(0);
    setC(0);
}
void cpu::op_aluorA(uint8_t val) {
    reg.A = reg.A | getR8L(val);
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
    setH(0);
    setC(0);
}
void cpu::op_alucpA(uint8_t val) {
    if (reg.A == getR8L(val)) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(1);
    setH(((reg.A & 0xf) - (getR8L(val) & 0xf)) & 0x10);
    if (reg.A < getR8L(val)) {
        setC(1);
    }
    else {
        setC(0);
    }
}
void cpu::returnStack() {
    uint8_t temp1 = memory[reg.SP++];
    uint8_t temp2 = memory[reg.SP++];
    pc = (((uint16_t)temp2 << 8) | temp1);
}
void cpu::op_retcond(uint8_t val) {
    if (getCondition(val)) {
        returnStack();
    }
}
void cpu::op_ldffuA() {
    memory[0xFF00+fetch()] = reg.A;
}
void cpu::op_addspi8() {
    reg.SP += fetchSigned();
}
void cpu::op_ldAffu() {
    reg.A = memory[0xFF00+fetch()];
}
void cpu::op_ldhlspi() {
    reg.HL = reg.SP + fetchSigned();
}
uint8_t& cpu::get3R16L(uint8_t val) {
    switch((val&0b00110000) >> 4) {
        case 0b00:
            return reg.C;
        case 0b01:
            return reg.E;
        case 0b10:
            return reg.L;
        case 0b11:
            return reg.F;
        default:
            cout << "Unknown r16" << endl;
            exit(26);
    }
}
uint8_t& cpu::get3R16U(uint8_t val) {
    switch((val&0b00110000) >> 4) {
        case 0b00:
            return reg.B;
        case 0b01:
            return reg.D;
        case 0b10:
            return reg.H;
        case 0b11:
            return reg.A;
        default:
            cout << "Unknown r16" << endl;
            exit(27);
    }
}
void cpu::op_popr16(uint8_t val) {
    get3R16L(val) = memory[reg.SP++];
    get3R16U(val) = memory[reg.SP++];
    reg.F = reg.F&0xF0;
}
void cpu::op_reti() {
    returnStack();
    op_ei();
}
void cpu::op_jphl() {
    pc = reg.HL;
}
void cpu::op_ldsphl() {
    reg.SP = reg.HL;
}
void cpu::op_ldffcA() {
    memory[0xFF00+reg.C] = reg.A;
}
void cpu::op_ldAffc() {
    reg.A = memory[0xFF00+reg.C];
}
void cpu::op_ldu16A() {
    memory[fetch16()] = reg.A;
}
void cpu::op_ldAu16() {
    reg.A = memory[fetch16()];
}
void cpu::jump16(uint16_t val) {
    pc = val;
}
void cpu::op_jpcond(uint8_t val) {
        uint16_t temp = fetch16();
    if (getCondition(val)) {
        jump16(temp);
    }
}
void cpu::op_di() {
    IME = 0;
}
void cpu::op_ei() {
    IME = 1;
}
void cpu::op_rlc(uint8_t val) {
    uint8_t temp = (getR8L(val) & 0x80) >> 7;
    getR8L(val) = getR8L(val) << 1;
    if (temp == 1) {
        getR8L(val) = getR8L(val) | 0x1;
    }
    setN(0);
    setH(0);
    if (getR8L(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setC(temp);
}
void cpu::op_rl(uint8_t val) {
    uint8_t temp = (getR8L(val) & 0x80) >> 7;
    getR8L(val) = getR8L(val) << 1;
    if (getC() == 1) {
        getR8L(val) = getR8L(val) | 0x1;
    }
    setN(0);
    setH(0);
    if (getR8L(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setC(temp);
}
void cpu::op_rr(uint8_t val) {
    uint8_t temp = (getR8L(val) & 0x1);
    getR8L(val) = getR8L(val) >> 1;
    if (getC() == 1) {
        getR8L(val) = getR8L(val) | 0b10000000;
    }
    setN(0);
    setH(0);
    if (getR8L(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setC(temp);
}
void cpu::op_rrc(uint8_t val) {
    uint8_t temp = (getR8L(val) & 0x1);
    getR8L(val) = getR8L(val) >> 1;
    if (temp == 1) {
        getR8L(val) = getR8L(val) | 0b10000000;
    }
    setN(0);
    setH(0);
    if (getR8L(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setC(temp);
}
void cpu::op_sla(uint8_t val) {
    setC(getR8L(val)&0b10000000 >> 7);
    getR8L(val) = getR8L(val) << 1;
    setH(0);
    setN(0);
    if (getR8L(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
}
void cpu::op_sra(uint8_t val) {
    setC(getR8L(val)&0b00000001);
    getR8L(val) = getR8L(val) >> 1;
    if (((getR8L(val)&0b01000000) >> 6 == 1)) {
        getR8L(val) = getR8L(val) | 0b10000000;
    }
    setH(0);
    setN(0);
    if (getR8L(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
}
void cpu::op_srl(uint8_t val) {
    setC(getR8L(val)&0b00000001);
    getR8L(val) = getR8L(val) >> 1;
    setH(0);
    setN(0);
    if (getR8L(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
}
void cpu::op_swap(uint8_t val) {
    uint8_t temp = getR8L(val);
    getR8L(val) = ((temp&0b00001111)<<4) | (temp>>4);
    if (getR8L(val) == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setC(0);
    setN(0);
    setH(0);
}
void cpu::op_bit(uint8_t val) {
    setZ(!getBit(getR8L(val), ((val&0b00111000) >> 3)));
    setN(0);
    setH(1);
}
void cpu::resetBit(uint8_t& regi, uint8_t num) {
    switch(num) {
        case 0:
            regi = regi&0b11111110;
            break;
        case 1:
            regi = regi&0b11111101;
            break;
        case 2:
            regi = regi&0b11111011;
            break;
        case 3:
            regi = regi&0b11110111;
            break;
        case 4:
            regi = regi&0b11101111;
            break;
        case 5:
            regi = regi&0b11011111;
            break;
        case 6:
            regi = regi&0b10111111;
            break;
        case 7:
            regi = regi&0b01111111;
            break;
        default:
            cout << "Unknown bit" << endl;
            exit(30);
    }
}
void cpu::op_res(uint8_t val) {
    resetBit(getR8L(val), ((val&0b00111000) >> 3));
}
void cpu::setBit(uint8_t& regi, uint8_t num) {
    switch(num) {
        case 0:
            regi = regi | 0b00000001;
            break;
        case 1:
            regi = regi | 0b00000010;
            break;
        case 2:
            regi = regi | 0b00000100;
            break;
        case 3:
            regi = regi | 0b00001000;
            break;
        case 4:
            regi = regi | 0b00010000;
            break;
        case 5:
            regi = regi | 0b00100000;
            break;
        case 6:
            regi = regi | 0b01000000;
            break;
        case 7:
            regi = regi | 0b10000000;
            break;
        default:
            cout << "Unknown bit" << endl;
            exit(31);
    }
}
void cpu::op_set(uint8_t val) {
    setBit(getR8L(val), ((val&0b00111000) >> 3));
}
void cpu::call(uint16_t val) {
    uint16_t temp = val;
    reg.SP--;
    memory[reg.SP--] = pc >> 8;
    memory[reg.SP] = pc&0x00FF;
    pc = temp;
}
void cpu::op_callcond(uint8_t val) {
    uint16_t temp = fetch16();
    if (getCondition(val)) {
        call(temp);
    }
}
void cpu::op_pushr16(uint8_t val) {
    reg.SP--;
    memory[reg.SP--] = get3R16U(val);
    memory[reg.SP] = get3R16L(val);
}
void cpu::op_callu16() {
    call(fetch16());
}
void cpu::op_aluaddu8() {
    uint8_t temp = fetch();
    setH(((reg.A & 0xf) + (temp & 0xf)) > 0xf);
    if ((reg.A + temp) > 0xFF) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.A += temp;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
}
void cpu::op_aluadcu8() {
    uint8_t temp = fetch();
    setH(((reg.A & 0xf) + (temp & 0xf) + getC()) > 0xf);
    uint8_t temp2 = getC();
    if ((reg.A + temp + getC()) > 0xFF) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.A += temp + temp2;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
}
void cpu::op_alusubu8() {
    uint8_t temp = fetch();
    setH(((reg.A & 0xf) - (temp & 0xf)) < 0);
    if (reg.A < temp) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.A -= temp;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(1);
}
void cpu::op_alusbcu8() {
    uint8_t temp = fetch();
    setH(((reg.A & 0xf) - (temp & 0xf) - getC()) < 0);
    uint8_t temp2 = getC();
    if (reg.A < (temp + getC())) {
        setC(1);
    }
    else {
        setC(0);
    }
    reg.A -= temp + temp2;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(1);
}
void cpu::op_aluandu8() {
    uint8_t temp = fetch();
    reg.A = reg.A&temp;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
    setH(1);
    setC(0);
}
void cpu::op_aluxoru8() {
    uint8_t temp = fetch();
    reg.A = reg.A ^ temp;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
    setH(0);
    setC(0);
}
void cpu::op_aluoru8() {
    uint8_t temp = fetch();
    reg.A = reg.A | temp;
    if (reg.A == 0) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(0);
    setH(0);
    setC(0);
}
void cpu::op_alucpu8() {
    uint8_t temp = fetch();
    if (reg.A == temp) {
        setZ(1);
    }
    else {
        setZ(0);
    }
    setN(1);
    setH(((reg.A & 0xf) - (temp & 0xf)) < 0);
    if (reg.A < temp) {
        setC(1);
    }
    else {
        setC(0);
    }
}
void cpu::op_aluau8(uint8_t val) {
    switch((val&0b00111000) >> 3) {
        case 0b000:
            op_aluaddu8();
            break;
        case 0b001:
            op_aluadcu8();
            break;
        case 0b010:
            op_alusubu8();
            break;
        case 0b011:
            op_alusbcu8();
            break;
        case 0b100:
            op_aluandu8();
            break;
        case 0b101:
            op_aluxoru8();
            break;
        case 0b110:
            op_aluoru8();
            break;
        case 0b111:
            op_alucpu8();
            break;
    }
}
void cpu::step() {
    if (memory[0xff02] == 0x81) {
        char c = memory[0xff01];
        printf("%c", c);
        memory[0xff02] = 0x0;
    }
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
                        case 0b011: //JR (unconditional)
                            op_jumpRel(fetchSigned());
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
                default: //LD r8, r8
                    op_ldr8r8(val);
                    break;
            }
            break;
        case 0b10: //ALU A, r8
            op_aluar8(val);
            break;
        case 0b11:
            switch(val&0b00000111) {
                case 0b000:
                    switch((val&0b00100000) >> 5) {
                        case 0b0: //RET condition
                            op_retcond(val);
                            break;
                        case 0b1:
                            switch((val&0b00011000) >> 3) {
                                case 0b00: //LD (FF00+u8), A
                                    op_ldffuA();
                                    break;
                                case 0b01: //ADD SP,i8
                                    op_addspi8();
                                    break;
                                case 0b10: //LD A, (FF00+u8)
                                    op_ldAffu();
                                    break;
                                case 0b11: //LD HL, SP+i8
                                    op_ldhlspi();
                                    break;
                            }
                            break;
                    }
                    break;
                case 0b001:
                    switch((val&0b00001000) >> 3) {
                        case 0b0: //POP r16
                            op_popr16(val);
                            break;
                        case 0b1: //opcode set
                            switch((val&0b00110000) >> 4) {
                                case 0b00: //RET
                                    returnStack();
                                    break;
                                case 0b01: //RETI
                                    op_reti();
                                    break;
                                case 0b10: //JP HL
                                    op_jphl();
                                    break;
                                case 0b11: //LD SP,HL
                                    op_ldsphl();
                                    break;
                            }
                            break;
                    }
                    break;
                case 0b010:
                    switch((val&0b00100000) >> 5) {
                        case 0b0: //JP condition
                            op_jpcond(val);
                            break;
                        case 0b1:
                            switch((val&0b00011000) >> 3) {
                                case 0b00: //LD (FF00+C), A
                                    op_ldffcA();
                                    break;
                                case 0b01: //LD (u16), A
                                    op_ldu16A();
                                    break;
                                case 0b10: //LD A, (FF00+C)
                                    op_ldAffc();
                                    break;
                                case 0b11: //LD A,(u16)
                                    op_ldAu16();
                                    break;
                            }
                            break;
                    }
                    break;
                case 0b011: //opcode set
                    switch((val&0b00111000) >> 3) {
                        case 0b000: //JP u16
                            jump16(fetch16());
                            break;
                        case 0b001:  { //(CB Prefix)
                            int val2 = fetch();
                            switch ((val2 & 0b11000000) >> 6) {
                                case 0b00: //Shifts/rotates {
                                    switch ((val2 & 0b00111000) >> 3) {
                                        case 0b000: //RLC
                                            op_rlc(val2);
                                            break;
                                        case 0b001: //RRC
                                            op_rrc(val2);
                                            break;
                                        case 0b010: //RL
                                            op_rl(val2);
                                            break;
                                        case 0b011: //RR
                                            op_rr(val2);
                                            break;
                                        case 0b100: //SLA
                                            op_sla(val2);
                                            break;
                                        case 0b101: //SRA
                                            op_sra(val2);
                                            break;
                                        case 0b110: //SWAP
                                            op_swap(val2);
                                            break;
                                        case 0b111: //SRL
                                            op_srl(val2);
                                            break;

                                    }
                                    break;
                                case 0b01: //BIT bit, r8
                                    op_bit(val2);
                                    break;
                                case 0b10: //RES bit, r8
                                    op_res(val2);
                                    break;
                                case 0b11: //SET bit, r8
                                    op_set(val2);
                                    break;
                            }
                            break;
                    }
                        case 0b110: //DI
                            op_di();
                            break;
                        case 0b111: //EI
                            op_ei();
                            break;
                        default:
                            cout << "Illegal Opcode" << endl;
                    }
                    break;
                case 0b100: //CALL condition
                    op_callcond(val);
                    break;
                case 0b101:
                    switch((val&0b00001000) >> 3) {
                        case 0b0: //PUSH r16
                            op_pushr16(val);
                            break;
                        case 0b1: //CALL u16
                            op_callu16();
                            break;
                    }
                    break;
                case 0b110: //ALU a,u8
                    op_aluau8(val);
                    break;
                case 0b111: //RST
                    call((val&0b00111000) >> 3);
                    break;
            }
            break;
    }
    timer();
}
void cpu::timer() {

}

cpu::cpu() = default;

registers::registers() {}
