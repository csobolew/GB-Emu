//
// Created by RealL on 9/7/2022.
//

#include "gb.h"
gb::gb(char* argv[]) {
    processor = cpu();
    load(argv);
}
void gb::load(char* argv[]) {
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
void gb::run() {
   ofstream outfile("log.txt");
    while(true) {
        processor.log(outfile);
        processor.step(memory);
    }
}