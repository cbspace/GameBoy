#pragma once

#include "memory.h"
#include "clock.h"

// Interrupt flags and interrupt enable mask values
#define I_VBLANK    0x01
#define I_LCDSTAT   0x02
#define I_TIMER     0x04
#define I_SERIAL    0x08
#define I_JOYPAD    0x10

// Interrupt Vectors
#define IV_VBLANK    0x040
#define IV_LCDSTAT   0x048
#define IV_TIMER     0x050
#define IV_SERIAL    0x058
#define IV_JOYPAD    0x060

// Special Memory Locations
#define A_IFLAGS    0xff0f              // Interrupt Flags location
#define A_IENABLE   0xffff              // Interrupt Enable location

class Interrupt
{
    public:
        Interrupt(Memory& mem_in, Clock& clk_in);        
        void check_interrupts();
        void cpu_halt();
        void cpu_stop();
        bool get_halt();
        bool get_stop();
        void disable_interrupts();
        void enable_interrupts();
        void if_update(uint8_t int_flag, bool flg_val);

        uint8_t ei_count;

    private:
        void cancel_stop();
        void process_ei_count();
        bool if_get(uint8_t int_flag);

        Memory& mem;
        Clock& clk;
        bool ime;
        bool halt_flag;
        bool stop_flag;
        uint8_t i_flags;        
        uint8_t i_enable;
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