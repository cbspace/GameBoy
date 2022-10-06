#include "Clock.h"

Clock::Clock() :
    line_clock_cycles(0)
{}

void Clock::add_cycles(u8 amount)
{
    line_clock_cycles += amount;
}

bool Clock::cycles_reached(DisplayMode display_mode)
{
        // OAM and Display RAM being used
        if ((display_mode == DisplayMode::Mode3) &&
            (line_clock_cycles >= CLK_CYCLES_MODE0 + CLK_CYCLES_MODE2 + CLK_CYCLES_MODE3))
                return true;
        // OAM being used
        else if ((display_mode == DisplayMode::Mode2) &&
            (line_clock_cycles >= CLK_CYCLES_MODE0 + CLK_CYCLES_MODE2))
                return true;
        // H-Blank
        else if ((display_mode == DisplayMode::Mode0) &&
            (line_clock_cycles >= CLK_CYCLES_MODE0))
                return true;

    return false;
}

void Clock::reset_cycles()
{
    line_clock_cycles = 0;
}  