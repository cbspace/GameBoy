#include "clock.h"
#include <stdint.h>
#include <SDL2/SDL.h>

// Constructor
Clock::Clock()
{

}

// Start the frame timer
void Clock::frame_timer_start()
{
    frame_start_ticks = SDL_GetTicks();
}

// SDL_Delay for remaining frame time
void Clock::frame_timer_delay()
{
    // Number of ticks elapsed this frame
    uint32_t frame_elap_ticks;
    frame_elap_ticks = SDL_GetTicks() - frame_start_ticks ;

    // Wait for remaining time
    SDL_Delay( CLK_FRAME_DELAY - frame_elap_ticks );
}
