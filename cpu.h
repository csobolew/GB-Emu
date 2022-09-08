//
// Created by Carson on 8/25/2022.
//

#ifndef GB_EMU_CPU_H
#define GB_EMU_CPU_H
#include <fstream>
#include <iostream>
#include <chrono>
using namespace std;

class cpu {
private:
    uint8_t fetch();
    uint16_t fetch16();
    static bool getBit(uint8_t reg, int num);
    void setZ(uint8_t v);
    uint8_t getZ() const;
    void setN(uint8_t v);
    uint8_t getN() const;
    void setH(uint8_t v);
    uint8_t getH() const;
    void setC(uint8_t v);
    uint8_t getC() const;
    int8_t fetchSigned();

    void op_jumpRel(int8_t val);
    void op_ldr16u16(uint8_t code);
    uint8_t& get1R16L(uint8_t code);
    uint8_t& get1R16U(uint8_t code);
    uint16_t& get1R16(uint8_t code);
    uint16_t& get2R16(uint8_t code);
    uint8_t& getR8(uint8_t code);
    uint8_t& getR8L(uint8_t code);
    void op_jumpRelCond(uint8_t code);
    void op_addHLr16(uint8_t code);
    void op_LDu16SP ();
    bool getCondition(uint8_t code);
    void op_ldr16A(uint8_t code);
    void op_ldAr16(uint8_t code);
    void op_INCr16(uint8_t code);
    void op_ldr8r8(uint8_t code);
    void op_DECr16(uint8_t code);
    void op_INCr8(uint8_t code);
    void op_DECr8(uint8_t code);
    void op_ldr8u8(uint8_t code);
    void op_group1(uint8_t code);
    void op_rlca();
    void op_aluaddA(uint8_t code);
    void op_aluadcA(uint8_t code);
    void op_alusubA(uint8_t code);
    void op_alusbcA(uint8_t code);
    void op_aluandA(uint8_t code);
    void op_aluxorA(uint8_t code);
    void returnStack();
    void op_aluorA(uint8_t code);
    void op_alucpA(uint8_t code);
    void op_aluar8(uint8_t code);
    void op_rrca();
    void op_rra();
    void op_scf();
    void op_daa();
    void op_ldffuA();
    void op_ldAffu();
    void op_ldhlspi();
    void op_popr16(uint8_t code);
    void op_rla();
    void op_ccf();
    void op_cpl();
    void op_addspi8();
    void op_retcond(uint8_t code);
    uint8_t& get3R16L(uint8_t code);
    uint8_t& get3R16U(uint8_t code);
    void op_jphl();
    void op_reti();
    void op_ldsphl();
    void op_ldffcA();
    void op_ldAffc();
    void op_ldu16A();
    void op_ldAu16();
    void op_jpcond(uint8_t code);
    void jump16(uint16_t code);
    void op_di();
    void op_ei();
    void op_rlc(uint8_t code);
    void op_rl(uint8_t code);
    void op_rrc(uint8_t code);
    void op_rr(uint8_t code);
    void op_sla(uint8_t code);
    void op_sra(uint8_t code);
    void op_srl(uint8_t code);
    void op_swap(uint8_t code);
    void op_bit(uint8_t code);
    void op_res(uint8_t code);
    static void resetBit(uint8_t& reg, uint8_t code);
    static void setBit(uint8_t& reg, uint8_t code);
    void op_set(uint8_t code);
    void op_callcond(uint8_t code);
    void call(uint16_t code);
    void op_pushr16(uint8_t code);
    void op_callu16();
    void op_aluaddu8();
    void op_aluadcu8();
    void op_alusubu8();
    void op_alusbcu8();
    void op_aluandu8();
    void op_aluxoru8();
    void op_aluoru8();
    void op_alucpu8();
    void op_aluau8(uint8_t code);
    void timer();
public:
    void log(ofstream& outfile);
    void step();
    cpu();
};


#endif //GB_EMU_CPU_H
