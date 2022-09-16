#include "clock.h"
#include "lib/Types.h"
#include <SDL2/SDL.h>

Clock::Clock() : line_clock_cycles(0)
{
	frame_start_ticks = SDL_GetTicks();
}

void Clock::frame_delay()
{
    u32 frame_elap_ticks;
    frame_elap_ticks = SDL_GetTicks() - frame_start_ticks ;

    if (frame_elap_ticks < CLK_FRAME_DELAY)
    {
        SDL_Delay(CLK_FRAME_DELAY - frame_elap_ticks);
    }

    frame_start_ticks = SDL_GetTicks();
}

void Clock::add_cycles(u8 amount)
{
    line_clock_cycles += amount;
}

bool Clock::cycles_reached(u8 display_mode)
{
    bool ret_val = false;

    switch(display_mode)
    {
        // OAM and Display RAM being used
        case CLK_DISPLAY_MODE3:
            if (line_clock_cycles >= CLK_CYCLES_MODE0 + CLK_CYCLES_MODE2 + CLK_CYCLES_MODE3)
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
        // H-Blank
        case CLK_DISPLAY_MODE0:
            if (line_clock_cycles >= CLK_CYCLES_MODE0)
            {
                ret_val = true;
            }
            break;
    }
    return ret_val;
}

void Clock::reset_cycles()
{
    line_clock_cycles = 0;
}  