#include "emulator.h"
#include "display.h"
#include "render.h"
#include "cpu.h"
#include "memory.h"
#include "interrupt.h"
#include "emudebug.h"
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
    // Initialise SDL
    cout << "Loading Window" << endl;
    disp.init();

    // Load the rom
    mem.load_rom(rom_path);

    // Set window title
    disp.set_title(mem.get_rom_title());
    cout << "Rom Title: " << mem.get_rom_title() << endl;

    if (rom_is_dmg) {
    	// Set the PC start address at 0
    	mem.set_pc(0x00);

    	// Insert logo for dmg rom
    	mem.debug_insert_logo();
    }

    // Run the main loop
    main_loop();
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

        if (clk.hblank_cycles_reached())
        {
        	// Draw current line
        	disp.update_line();

            // TEMP slow down
            //SDL_Delay(1);
        }

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
