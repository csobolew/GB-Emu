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

unsigned char cpu::fetch() {
    unsigned char temp = memory[pc];
    pc++;
    return temp;
}
pair<unsigned char, unsigned char> cpu::fetch16() {
    unsigned char temp1 = fetch();
    unsigned char temp2 = fetch();
    return make_pair(temp1, temp2);
}
void cpu::step() {
    switch(fetch()) {

        case 0x00:
            break;
        case 0x01:
            reg.C = fetch();
            reg.B = fetch();
            break;
        case 0x21:
            reg.L = fetch();
            reg.H = fetch();
            break;
        case 0x31: {
            pair<unsigned char, unsigned char> temp = fetch16();
            int temp2 = (int)temp.second << 8;
            sp = temp2 | (int)temp.first;
            break;
        }
        case 0x32:

        case 0xAF: {
            if ((reg.A ^ reg.A) == 0) {
                flag.Z = 1;
            }
            reg.A = reg.A ^ reg.A;
            break;
        }


        //CB-prefix codes
        case 0xCB:
            switch(memory[pc+1]) {

            }
            break;
        default:
            cout << "Unknown Opcode " << memory[pc] << " at " << pc << endl;
            exit(1);
    }
}