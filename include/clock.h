#ifndef CLOCK_H
#define CLOCK_H

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

VBLANK PERIOD: 59.73 Hz for Gameboy and 61.1 Hz for Super Gameboy

Frame Draw Timing   Machine Cycles
Line (BG)           43
Line (Sprites)      20
H-Blank             51* 7,293
V-Blank             1140*
Total               1254

Screen refresh every 17,556* cycles
*/

#define CLK_MAIN_DELAY              16         // Clock period for main clock (in ms), results in 62 Hz Clock
#define CLK_CPU_CYCLES_MAIN         9123       // ( 1,050,000 / 59.73 ) - H-Blank * 143 - VBlank
// This is the number of CPU cycles to execute between in the main timing loop

class Clock
{
    public:
        Clock();
};

#endif // CLOCK_H
