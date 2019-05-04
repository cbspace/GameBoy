#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.h"
#include "display.h"
#include "memory.h"
#include <string>
using namespace std;

static const float RELEASE_VERSION = 0.13;

class Emulator
{
    public:
        string rom_title;   // Game title stored in the rom
        Emulator();         // Constructor
        void init();        // Creates display, cpu and memory
        void start(char* rom_path);             // Init SDL and load the ROM
        void print_error(string error_string);  // Print error to std_out
    private:
        Display disp;
        Cpu cp;
        Memory mem;
};

#endif // EMULATOR_H
