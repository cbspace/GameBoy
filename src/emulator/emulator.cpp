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

// Constructor
Emulator::Emulator()
{
    // Pass pointers to CPU instance
    cp.attach_memory(&mem);
    cp.attach_clock(&clk);
    cp.attach_interrupt(&ir);
    cp.attach_emudebug(&edb);

    // Pass pointers to Interrupt instance
    ir.attach_memory(&mem);
    ir.attach_clock(&clk);

    // Pass pointers to memory Display instance
    disp.attach_memory(&mem);
    disp.attach_render(&ren);
    disp.attach_interrupt(&ir);
    disp.attach_clock(&clk);

    // Pass pointer to Memory instance to Render instance
    ren.attach_memory(&mem);
	
    // Pass pointer to Memory instance to Joypad instance
    jp.attach_memory(&mem);

    // Pass pointer to Interrupt instance to Joypad instance
    jp.attach_interrupt(&ir);

	// Pass pointer to Memory instance to Emudebug instance
    edb.attach_memory(&mem);

    // Flag to signify if the user wishes to quit
    quit_flag = false;
}

// Run the emulator
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
    }
    else {
        disp.set_title(mem.get_rom_title());
        cout << "Rom Title: " << mem.get_rom_title() << endl;

        mem.write_byte(R_LCDC, 0x91);
    }

    main_loop();
}

// The main emulator loop
void Emulator::main_loop()
{
    // Continue to loop until the user quits
    while (!quit_flag)
    {
        // Check for key press events
        quit_flag = jp.key_down();

        // Perform CPU cycles
        cp.cycle();

        // DEBUG - Detect CPU Runaway
        if (edb.detect_runaway())
        {
        	//quit_flag = true;
        }

        // Update the display
        disp.display_cycle();

        //mem.debug_sprite_test();

        // Check for interrupts and process accordingly
        ir.check_interrupts();
    }
}