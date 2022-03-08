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
    // Pass pointer to memory object to CPU object
    cp.attach_memory(&mem);

    // Pass pointer to clock object to CPU object
    cp.attach_clock(&clk);

    // Pass pointer to interrupt object to CPU object
    cp.attach_interrupt(&ir);

    // Pass pointer to debug object to CPU object
    cp.attach_emudebug(&edb);

    // Pass pointer to memory object to interrupt object
    ir.attach_memory(&mem);

    // Pass pointer to clock object to interrupt object
    ir.attach_clock(&clk);

    // Pass pointer to display object to interrupt object
    //ir.attach_display(&disp);

    // Pass pointer to memory object to display object
    disp.attach_memory(&mem);

    // Pass pointer to render object to display object
    disp.attach_render(&ren);

    // Pass pointer to interrupt object to Display object
    disp.attach_interrupt(&ir);

    // Pass pointer to memory object to render object
    ren.attach_memory(&mem);
	
	// Pass pointer to memory object to emudebug object
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

    // Set initial value of V_BLANK interrupt enable (on)
    mem.write_byte(A_IENABLE,I_VBLANK);

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

        if (clk.hblank_cycles_reached())
        {
            // Update LCDSTAT register (clear H-Blank)


        	// Draw current line
        	disp.update_line();
        }

        // TEMP slow down
        //SDL_Delay(1);

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
