#include "clock.h"
#include <stdint.h>
#include <SDL2/SDL.h>

// Constructor empty
Clock::Clock() { }

// Start the frame timer and reset clock cycles
void Clock::frame_start()
{
    frame_start_ticks = SDL_GetTicks();
    frame_clock_cycles = 0;
}

// SDL_Delay for remaining frame time
void Clock::frame_delay()
{
    // Number of ticks elapsed this frame
    uint32_t frame_elap_ticks;
    frame_elap_ticks = SDL_GetTicks() - frame_start_ticks ;

    // Check if frame is finished early
    if ( frame_elap_ticks < CLK_FRAME_DELAY )
    {
        // Wait for remaining time
        SDL_Delay( CLK_FRAME_DELAY - frame_elap_ticks );
    }
}

// Add cycles to frame_clock_cycles (4Mhz Clock)
void Clock::add_cycles(uint8_t amount)
{
    frame_clock_cycles += amount;
}

// Indicates if max clock cycles for a single frame is reached
bool Clock::max_cycles_reached()
{
    if ( frame_clock_cycles < CLK_CYCLES_MAX )
    {
        return false;
    }
    else
    {
        return true;
    }
}

// Temp: Indicates if number of cycles before VBLNK is reached
bool Clock::vblank_cycles_reached()
{
    if ( frame_clock_cycles > (CLK_CYCLES_MAX - CLK_CYCLES_VBLANK ) )
    {
        return true;
    }
    else
    {
        return false;
    }
}
