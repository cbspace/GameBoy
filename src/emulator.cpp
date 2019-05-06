#include "emulator.h"
#include "display.h"
#include "cpu.h"
#include "memory.h"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

// Constructor
Emulator::Emulator()
{
    // Pass pointer to memory object to CPU
    cp.attach_memory(&mem);
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

    // Run the main loop
    main_loop();
}

// The main emulator loop
void Emulator::main_loop()
{
    // Flag to signify if the user wishes to quit
    bool quit_flag = false;

    // SDL event handler
    SDL_Event e;

    // Continue to loop until the user quits
    while (!quit_flag)
    {
        // Start the frame timer
        clk.frame_timer_start();

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

        // Perform CPU cycle
        cp.cycle();

        // Delay until next frame
        clk.frame_timer_delay();
    }
}

// Function to print error message
void Emulator::print_error(string error_string) {
    cout << "Error: " << error_string << endl;
}
