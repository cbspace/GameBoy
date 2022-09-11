#include "emulator.h"
#include "../display/display.h"
#include "../render/render.h"
#include "../cpu/cpu.h"
#include "../memory/memory.h"
#include "../interrupt/interrupt.h"
#include "../emudebug/emudebug.h"
#include "../clock/clock.h"
#include "../joypad/joypad.h"

#include <iostream>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

Emulator::Emulator()
{
    cp.attach_memory(&mem);
    cp.attach_clock(&clk);
    cp.attach_interrupt(&ir);
    cp.attach_emudebug(&edb);
    ir.attach_memory(&mem);
    ir.attach_clock(&clk);
    disp.attach_memory(&mem);
    disp.attach_render(&ren);
    disp.attach_interrupt(&ir);
    disp.attach_clock(&clk);
    ren.attach_memory(&mem);
    jp.attach_memory(&mem);
    jp.attach_interrupt(&ir);
    edb.attach_memory(&mem);
    quit_flag = false;
}

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
        mem.debug_insert_logo();
    } else {
        disp.set_title(mem.get_rom_title());
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
        //mem.debug_sprite_test();
        ir.check_interrupts();
    }
}

void Emulator::print_cl_usage_message() {
    cout << "Command line usage:" << endl;
    cout << "(1)  gameboy /path/to/rom.gb [-debug]" << endl;
    cout << "(2)  gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]" << endl;
}