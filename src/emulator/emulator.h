#pragma once

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

class Emulator
{
    public:
        Emulator();
        void start(char* rom_path, bool rom_is_dmg, bool debug_mode_enabled);
        void main_loop();
        void print_cl_usage_message();
        string rom_title;

    private:
        Display disp;
        Cpu cp;
        Render ren;
        Memory mem;
        Clock clk;
        Interrupt ir;
        Emudebug edb;
        Joypad jp;
        bool quit_flag;
};