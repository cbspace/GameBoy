#ifndef EMULATOR_H
#define EMULATOR_H

#include "../display/display.h"
#include "../render/render.h"
#include "../cpu/cpu.h"
#include "../memory/memory.h"
#include "../interrupt/interrupt.h"
#include "../emudebug/emudebug.h"
#include "../clock/clock.h"
#include "../joypad/joypad.h"
#include <string>
using namespace std;

static const string RELEASE_VERSION = "0.45.2";

class Emulator
{
    public:
        string rom_title;                       // Game title stored in the rom
        Emulator();                             // Constructor
                                                // Init SDL and load the ROM
        void start(char* rom_path, bool rom_is_dmg, bool debug_mode_enabled);
        void main_loop();                       // The main emulator loop
    private:
        Display disp;                           // Main SDL display
        Cpu cp;                                 // CPU for the emulator
        Render ren;                             // Renderer		
        Memory mem;                             // Memory including registers
        Clock clk;                              // Clock for emulator
        Interrupt ir;                           // Interrupt class object
        Emudebug edb;                           // Emudebug class object
        Joypad jp;                              // Joypad class object
        bool quit_flag;                         // Flag to signify if the user wishes to quit
};

#endif // EMULATOR_H
