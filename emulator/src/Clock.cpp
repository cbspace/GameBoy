#include "Clock.h"

// Game Boy dot timings:
// Mode 2: 80 dots
// Mode 3: 172 dots  -> cumulative: 80 + 172 = 252 dots
// Mode 0: 204 dots  -> cumulative: 252 + 204 = 456 dots (full scanline)
// Mode 1: 456 dots per V-Blank line

Clock::Clock() :
    line_clock_cycles(0)
{}

void Clock::add_cycles(u8 amount)
{
    line_clock_cycles += amount;
}

bool Clock::cycles_reached(DisplayMode display_mode) const
{
    switch (display_mode)
    {
    case DisplayMode::Mode2:
        // Mode 2 ends at 80 dots
        return line_clock_cycles >= 80;

    case DisplayMode::Mode3:
        // Mode 3 ends at 80 + 172 = 252 dots
        return line_clock_cycles >= (80 + 172);

    case DisplayMode::Mode0:
        // Mode 0 ends at 456 dots (full scanline complete)
        return line_clock_cycles >= 456;

    case DisplayMode::Mode1:
        // Each V-Blank line (lines 144-153) lasts a full scanline (456 dots)
        return line_clock_cycles >= 456;

    default:
        return false;
    }
}

void Clock::reset_cycles()
{
    line_clock_cycles = 0;
}