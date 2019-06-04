#include "emulator.h"
#include "display.h"
#include "cpu.h"
#include "memory.h"
#include "interrupt.h"
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

    // Pass pointer to memory object to interrupt object
    ir.attach_memory(&mem);

    // Pass pointer to clock object to interrupt object
    ir.attach_clock(&clk);

    // Flag to signify if the user wishes to quit
    quit_flag = false;
}

// Run the emulator
void Emulator::start(char* rom_path)
{
    // Initialise SDL
    cout << "Loading Window" << endl;
    disp.init();

    // Load the rom
    mem.load_rom(rom_path);
    disp.set_title(mem.get_rom_title());
    cout << "Rom Title: " << mem.get_rom_title() << endl;

    /// Temp set initial ie value
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
        // Start the frame timer and reset clock cycles
        clk.frame_start();

        // Check for key press events
        key_down();

        while (!clk.max_cycles_reached())
        {
            // Set VBLANK interrupt flag
            if (clk.vblank_cycles_reached())
            {
                ir.if_update(I_VBLANK, true);
            }

            // Perform CPU cycles
            cp.cycle();

            // Check for interrupts and process accordingly
            ir.check_interrupts();
        }

        // Clear VBLANK interrupt flag
        ir.if_update(I_VBLANK, false);

        // Draw the frame!
        disp.render_frame();

        // Delay until next frame
        clk.frame_delay();
    }
}

// Process key press events
void Emulator::key_down()
{
    // SDL event handler
    SDL_Event e;

    // Handle events on the event queue
    while ( SDL_PollEvent( &e ) != 0 )
    {
        // User requests to quit
        if ( e.type == SDL_QUIT )
        {
            quit_flag = true;
        }
        else if ( e.type == SDL_KEYDOWN )
        {
            if ( e.key.keysym.sym == SDLK_ESCAPE )
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
