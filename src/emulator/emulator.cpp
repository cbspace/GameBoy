#include "emulator.h"
#include "../display/display.h"
#include "../render/render.h"
#include "../cpu/cpu.h"
#include "../memory/memory.h"
#include "../interrupt/interrupt.h"
#include "../emudebug/emudebug.h"
#include "../clock/clock.h"
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
	
	// Pass pointer to Memory instance to Emudebug instance
    edb.attach_memory(&mem);

    // Flag to signify if the user wishes to quit
    quit_flag = false;
}

// Run the emulator
void Emulator::start(char* rom_path, bool rom_is_dmg)
{
    // Load the rom
    uint8_t load_rom_status = mem.load_rom(rom_path);

    if (load_rom_status > 0)
    {
        // Initialise SDL
        cout << "Loading Window" << endl;
        disp.init();

        if (rom_is_dmg) {
            // Set the PC start address at 0
            mem.set_pc(0x00);

            // Insert logo for dmg rom
            mem.debug_insert_logo();
        }
        else {
            // Set window title
            disp.set_title(mem.get_rom_title());
            cout << "Rom Title: " << mem.get_rom_title() << endl;

            // Initialise LCDC
            mem.write_byte(R_LCDC, 0x91);
        }

        // Run the main loop
        main_loop();
    }
}

// The main emulator loop
void Emulator::main_loop()
{
    // Continue to loop until the user quits
    while (!quit_flag)
    {
        // Check for key press events
        key_down();

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

// Process key press events
void Emulator::key_down()
{
    // SDL event handler
    SDL_Event e;

    // Handle events on the event queue
    while (SDL_PollEvent(&e) != 0)
    {
        // User requests to quit
        if (e.type == SDL_QUIT)
        {
            quit_flag = true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit_flag = true;
            }
        }
    }
}

// Function to print error message
void Emulator::print_error(string error_string) {
    cout << "Error: " << error_string << endl;
}
