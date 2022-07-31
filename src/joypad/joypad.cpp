#include "joypad.h"
#include "../memory/memory.h"

#include <SDL2/SDL.h>

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
            quit_signal = true;
        }
        else if (e.type == SDL_KEYDOWN)
        {
            if (e.key.keysym.sym == SDLK_ESCAPE)
            {
                quit_signal = true;
            }
        }
    }
    return quit_signal;
}