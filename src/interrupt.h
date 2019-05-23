#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "memory.h"
#include "clock.h"

// Constants



class Interrupt
{
    public:
        Interrupt();
        void attach_memory(Memory* mem_in);     // Set pointer used to access memory object
        void attach_clock(Clock* clk_in);       // Set pointer used to access clock object
        void cpu_halt();                        // Halt the CPU until an interrupt occurs (HALT)
        void cpu_stop();                        // Stop the CPU and LCD until a button is pressed (STOP)
        bool get_halt();                        // Get value of CPU halt flag
        bool get_stop();                        // Get value of CPU stop flag

    private:
        Memory* mem;                            // Pointer to memory object
        Clock* clk;                             // Pointer to clock object
        bool ime;                               // Interrupt master enable
        bool halt_flag;                         // CPU halt flag (true = halted)
        bool stop_flag;                         // CPU stop flag (true = halted)

        void cancel_stop();                     // Cancel CPU stop
};

/* Interrupt Registers

Interrupt Master Enable (IME) - Enabled by default
0: Disable all interrupts (Reset by DI instruction)
1: Enable interrupts (Set by EI instruction)

Interrupt Flags (IF) Location FF0F (1 = Enabled, 0 = Disabled)
Bit 7 - Not Used
Bit 6 - Not Used
Bit 5 - Not Used
Bit 4 - P10 - P13 Negative Edge (For Joypad)
Bit 3 - Serial I/O Transfer Complete
Bit 2 - Timer Overflow
Bit 1 - LCDC (STAT Referenced)
Bit 0 - Vertical Blanking

Interrupt Enable (IE) Location FFFF (1 = Enabled, 0 = Disabled)
Bit 7 - Not Used
Bit 6 - Not Used
Bit 5 - Not Used
Bit 4 - P10 - P13 Negative Edge (For Joypad)
Bit 3 - Serial I/O Transfer Complete
Bit 2 - Timer Overflow
Bit 1 - LCDC (STAT Referenced)
Bit 0 - Vertical Blanking

*/

#endif // INTERRUPT_H
