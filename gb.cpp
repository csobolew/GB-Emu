//
// Created by RealL on 9/7/2022.
//

#include "gb.h"
gb::gb(char* argv[]) {
    mem.load(argv);
}
void gb::run() {
   ofstream outfile("log.txt");
    while(true) {
        processor.log(outfile);
        if(processor.getHalted()) {
            cycles = 4;
        }
        else {
            cycles = processor.step();
        }
        tim.updateTimer(cycles);
        processor.checkInterrupts();
    }
}