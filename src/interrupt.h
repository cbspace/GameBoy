#pragma once

#include "Memory.h"
#include "Clock.h"
#include "LibCB/Types.h"
#include <iostream>

// Interrupt flags and interrupt enable mask values
const u32 I_VBLANK   =   0x01;
const u32 I_LCDSTAT  =   0x02;
const u32 I_TIMER    =   0x04;
const u32 I_SERIAL   =   0x08;
const u32 I_JOYPAD   =   0x10;

// Interrupt Vectors
const u32 IV_VBLANK  =   0x040;
const u32 IV_LCDSTAT =   0x048;
const u32 IV_TIMER   =   0x050;
const u32 IV_SERIAL  =   0x058;
const u32 IV_JOYPAD  =   0x060;

// Special Memory Locations
const u32 A_IFLAGS   =   0xff0f;
const u32 A_IENABLE  =   0xffff;

class Interrupt
{
    public:
        Interrupt(Memory& mem_in);        
        void check_interrupts();
        bool get_ime();
        void disable_interrupts();
        void enable_interrupts();
        void if_update(u8 int_flag, bool flg_val);

        u8 ei_count;

    private:
        void process_ei_count();
        bool if_get(u8 int_flag);

        Memory& mem;
        bool ime;
        // bool halt_flag;
        // bool stop_flag;
        u8 i_flags;        
        u8 i_enable;
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