#pragma once

#include "display.h"
#include "render.h"
#include "cpu.h"
#include "Memory.h"
#include "interrupt.h"
#include "SystemDebug.h"
#include "Timer.h"
#include "joypad.h"
#include "lib/Error.h"
#include <iostream>
#include <string>
#include <optional>

using namespace std;

class System
{
    public:
        System();
        optional<Error> start(string rom_path, bool rom_is_dmg, bool debug_mode_enabled);
        void main_loop();
        void print_cl_usage_message();
        string rom_title;
        u32* get_display_buffer();
        bool new_frame_is_drawn();

        Joypad jpad;
        SystemDebug debug;
    private:
        GBDisplay disp;
        Interrupt interrupt;
        Timer timer;
        Memory memory;
        Cpu cpu;
        bool quit_flag;
};
