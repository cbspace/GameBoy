#pragma once

#include "display.h"
#include "render.h"
#include "cpu.h"
#include "Memory.h"
#include "interrupt.h"
#include "emudebug.h"
#include "Clock.h"
#include "joypad.h"
#include "lib/Error.h"
#include <iostream>
#include <string>
#include <optional>

using namespace std;

class Emulator
{
    public:
        Emulator();
        optional<Error> start(string rom_path, bool rom_is_dmg, bool debug_mode_enabled);
        void main_loop();
        void print_cl_usage_message();
        string rom_title;

        GBDisplay disp;
    private:
        Cpu cpu;
        Memory memory;
        Clock clock;
        Interrupt interrupt;
        Emudebug emudebug;
        // Joypad jp;
        bool quit_flag;
};
