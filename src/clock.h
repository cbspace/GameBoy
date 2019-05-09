#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

/*
The machine clock for a gameboy is 1.050 MHz and the base clock is 4.19 MHz.
It is not practical to reliably clock the emulator at such high clock rates on a PC.
Although the PC has a much higher clock rate, the clock rate varies between PCs and
the time slices available vary between operating systems. The period of the time slices
offered by the OS's scheduler are also comparable in duration to the times needed for
such high frequency clocks.

As a result, it was decided to use a 60hz clock, to synchronise the clock with the
vertical refresh rate (V-Blank). Within each iteration the appropriate number of CPU
iterations are executed. Note - this method does not result in a perfect representation
of H-Blank as the timing would be compressed. It is not expected that this discrepency
will be noticiable and most games don't even utilise H-Blank updates.

VBLANK Frequency: 59.73 Hz for Gameboy and 61.1 Hz for Super Gameboy
VBLANK lasts ~1.1ms or 4560 clks (from manual)
HBLANK lasts 204 clks
Complete screen refresh takes 70,224 clock ticks

Frame Draw Timing   Machine Cycles      Clock Cycles
Line (BG)           43 x 144 = 172      24,768
Line (Sprites)      20 x 144 = 2,880    11,520
H-Blank             51 x 144 = 7,344    29,376
V-Blank             1,140               4,560
Total               17,556              70,224

*/

#define CLK_FRAME_DELAY          500 //16    // Clock period for frame clock (in ms), results in ~60 Hz Clock
#define CLK_CYCLES_MAX           70224       // Maximum clock cycles per frame

class Clock
{
    public:
        Clock();
        void frame_start();                 // Start the frame timer and reset clock
        void frame_delay();                 // SDL_Delay for remaining frame time
        void add_cycles(uint8_t amount);    // Add cycles to frame_clock_cycles
        bool max_cycles_reached();          // Indicates if max clock cycles for a single frame is reached
    private:
        uint32_t frame_start_ticks;         // Start time of frame
        uint32_t frame_clock_cycles;        // Number of clk cycles in current frame
};

#endif // CLOCK_H
