#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.h"
#include "display.h"
#include "memory.h"
#include <string>
using namespace std;

static const float RELEASE_VERSION = 0.11;

class Emulator
{
    public:
        Emulator();
        void init();
        void start();
        void print_error(string error_string);
    private:
        Display* disp;
        Cpu* cp;
        Memory* mem;
};

#endif // EMULATOR_H
