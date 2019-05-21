#ifndef EMULATOR_H
#define EMULATOR_H

#include "cpu.h"
#include "display.h"
#include "memory.h"
#include "clock.h"
#include <string>
using namespace std;

static const string RELEASE_VERSION = "0.25";

class Emulator
{
    public:
        string rom_title;                       // Game title stored in the rom
        Emulator();                             // Constructor
        void start(char* rom_path);             // Init SDL and load the ROM
        void main_loop();                       // The main emulator loop
        void print_error(string error_string);  // Print error to std_out
    private:
        Display disp;                           // Main SDL display
        Cpu cp;                                 // CPU for the emulator
        Memory mem;                             // Memory including registers
        Clock clk;                              // Clock for emulator
};

#endif // EMULATOR_H
