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

CPU and Frame Timing:
1. Perform 65,664 CPU cycles {1} (Draw backgound line and sprite line and H Blank)
2. Will need to set H-Blank ISR flag every 456 cycles ({1}/144) after each line
3. Once 65,664 cycles reached, draw the frame
4. Set V-Blank interrupt and flags
5. Perform 4,560 cycles
6. Do it all again!
* HBLANK not used yet, just run 65,664 cycles then draw frame, do VBLANK
  VBLANK cycles are not counted, will run till finished

Will need to incorporate interpolation of drawing line and executing
CPU instructions in order for progrms using LY register to work

VBLANK Frequency: 59.73 Hz for Gameboy and 61.1 Hz for Super Gameboy
VBLANK lasts ~1.1ms or 4560 clks (from manual)
HBLANK lasts 204 clks
Complete screen refresh takes 70,224 clock ticks

Frame Draw Timing   Machine Cycles      Clock Cycles _
Line (BG)           43 x 144 = 172      24,768        |
Line (Sprites)      20 x 144 = 2,880    11,520        | {1}
H-Blank             51 x 144 = 7,344    29,376       _|
V-Blank             1,140               4,560
Total               17,556              70,224

*/

#define CLK_FRAME_DELAY          200//16          // Clock period for frame clock (in ms), results in ~60 Hz Clock
#define CLK_CYCLES_MAX           70224       // Maximum clock cycles per frame
#define CLK_CYCLES_LINE          252         // Clock cycles for a single line BG + Sprites
#define CLK_CYCLES_HBLANK        204         // Clock cycles for a single line HBLANK period
#define CLK_CYCLES_VBLANK        4560        // Clock cycles for a single line VBLANK period

class Clock
{
    public:
        Clock();
        void frame_start();                 // Start the frame timer and reset clock
        void frame_delay();                 // SDL_Delay for remaining frame time
        void add_cycles(uint8_t amount);    // Add cycles to frame_clock_cycles
        bool max_cycles_reached();          // Indicates if max clock cycles for a single frame is reached
        bool vblank_cycles_reached();       // Temp: Indicates if number of cycles before VBLNK is reached
    private:
        uint32_t frame_start_ticks;         // Start time of frame
        uint32_t frame_clock_cycles;        // Number of clk cycles in current frame
};

#endif // CLOCK_H
