#include "emulator.h"
#include "display.h"
#include "render.h"
#include "cpu.h"
#include "memory.h"
#include "interrupt.h"
#include "emudebug.h"
#include "clock.h"
#include "joypad.h"

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

Emulator::Emulator() : 
    cp(mem, clk, ir, edb),
    disp(mem, ir, clk),
    ir(mem, clk),
    jp(mem, ir),
    edb(mem),
    quit_flag(false)
{}

void Emulator::start(char* rom_path, bool rom_is_dmg, bool debug_mode_enabled)
{
    if ( auto e = mem.load_rom(rom_path) ) { 
        cout << e.value().get_error_string() << endl;;
        return; 
    }

    cout << "Loading Window" << endl;
    if( auto e = disp.init() ) {
        cout << e.value().get_error_string() << endl;
        return;
    }

    if (rom_is_dmg) {
        mem.set_pc(0x00);
        edb.insert_logo();
    } else {
        disp.set_window_title(mem.get_rom_title());
        cout << "Rom Title: " << mem.get_rom_title() << endl;
        mem.write_byte(R_LCDC, 0x91);
    }

    main_loop();
}

void Emulator::main_loop()
{
    while (!quit_flag)
    { 
        quit_flag = jp.key_down(); // || edb.detect_runaway();

        cp.cycle();
        disp.display_cycle();
        ir.check_interrupts();
    }
}

void Emulator::print_cl_usage_message() {
    cout << "Command line usage:" << endl;
    cout << "(1)  gameboy /path/to/rom.gb [-debug]" << endl;
    cout << "(2)  gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]" << endl;
}
