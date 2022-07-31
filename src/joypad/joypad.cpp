#include "joypad.h"
#include "../memory/memory.h"

#include <SDL2/SDL.h>
#include <iostream>

Joypad::Joypad()
{
    mem = NULL;
}

// Set pointer used to access memory object
void Joypad::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

// Process key press events, return 1 on quit
uint8_t Joypad::key_down()
{
    // SDL event handler
    SDL_Event e;

    // Quit signal received
    uint8_t quit_signal = 0;

    // Handle events on the event queue
    while (SDL_PollEvent(&e) != 0)
    {
        // User requests to quit
        if (e.type == SDL_QUIT)
        {
            quit_signal = 1;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            switch (e.key.keysym.sym)
            {
                case SDLK_ESCAPE:
                {
                    quit_signal = 1;
                }
                case SDLK_RETURN:
                {
                    mem->write_byte(R_P1,JP_START);
                }
                default:
                {
                    mem->write_byte(R_P1,JP_EMPTY);
                }
            }
        }
    }
    return quit_signal;
}