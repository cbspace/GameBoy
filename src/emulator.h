#pragma once

#include "display.h"
#include "render.h"
#include "cpu.h"
#include "memory.h"
#include "interrupt.h"
#include "emudebug.h"
#include "clock.h"
#include "joypad.h"
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
        Memory mem;
        Clock clk;
        Interrupt ir;
        Emudebug edb;
        Joypad jp;
        bool quit_flag;
};
