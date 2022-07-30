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

// Indicates if number of cycles is reached
// When
bool Clock::cycles_reached(uint8_t display_mode)
{
    bool ret_val = false;

    switch(display_mode)
    {
        // H-Blank
        case CLK_DISPLAY_MODE0:
            if (line_clock_cycles >= CLK_CYCLES_MODE0)
            {
                ret_val = true;
            }
            break;
        // V-Blank
        case CLK_DISPLAY_MODE1:
            if (line_clock_cycles >= CLK_CYCLES_MODE0 + CLK_CYCLES_MODE2 + CLK_CYCLES_MODE3 + CLK_CYCLES_MODE1)
            {
                ret_val = true;
            }
            break;
        // OAM being used
        case CLK_DISPLAY_MODE2:
            if (line_clock_cycles >= CLK_CYCLES_MODE0 + CLK_CYCLES_MODE2)
            {
                ret_val = true;
            }
            break;
        // OAM and Display RAM being used
        case CLK_DISPLAY_MODE3:
            if (line_clock_cycles >= CLK_CYCLES_MODE0 + CLK_CYCLES_MODE2 + CLK_CYCLES_MODE3)
            {
                ret_val = true;
            }
            break;
    }
    return ret_val;
}

// Reset line_clock_cycles
void Clock::reset_cycles()
{
    line_clock_cycles = 0;
}  