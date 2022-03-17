#include "clock.h"
#include <stdint.h>
#include <SDL2/SDL.h>

// Constructor empty
Clock::Clock()
{
	frame_start_ticks = SDL_GetTicks();
	line_clock_cycles = 0;
}

// SDL_Delay for remaining frame time
void Clock::frame_delay()
{
    // Number of ticks elapsed this frame
    uint32_t frame_elap_ticks;
    frame_elap_ticks = SDL_GetTicks() - frame_start_ticks ;

    // Check if frame is finished early
    if (frame_elap_ticks < CLK_FRAME_DELAY)
    {
        // Wait for remaining time
        SDL_Delay(CLK_FRAME_DELAY - frame_elap_ticks);
    }

    // Reset counter
    frame_start_ticks = SDL_GetTicks();
}

// Add cycles to frame_clock_cycles (4Mhz Clock)
void Clock::add_cycles(uint8_t amount)
{
    line_clock_cycles += amount;
}

// Indicates if number of cycles before HBLNK is reached
bool Clock::hblank_cycles_reached()
{
    if (line_clock_cycles >= CLK_CYCLES_LINE)
    {
        line_clock_cycles = 0;
        return true;
    }
    else
    {
        return false;
    }
}
