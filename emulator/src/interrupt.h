#pragma once

#include "Memory.h"
#include "lib/Types.h"
#include <iostream>

// Strongly-typed enum for interrupt sources
enum class InterruptType {
    VBlank,
    LCDStat,
    Timer,
    Serial,
    Joypad
};

// Interrupt flags and interrupt enable mask values
const u8 I_VBLANK   = 0x01;
const u8 I_LCDSTAT  = 0x02;
const u8 I_TIMER    = 0x04;
const u8 I_SERIAL   = 0x08;
const u8 I_JOYPAD   = 0x10;

// Interrupt Vectors
const u16 IV_VBLANK  = 0x0040;
const u16 IV_LCDSTAT = 0x0048;
const u16 IV_TIMER   = 0x0050;
const u16 IV_SERIAL  = 0x0058;
const u16 IV_JOYPAD  = 0x0060;

// Special Memory Locations
const u16 A_IFLAGS   = 0xFF0F;
const u16 A_IENABLE  = 0xFFFF;

class Interrupt
{
public:
    Interrupt(Memory& mem_in);        
    void check_interrupts();
    bool get_ime();
    void disable_interrupts();
    void enable_interrupts();
    void if_update(u8 int_flag, bool flg_val);

    // Call from PPU, Timer, Joypad, Serial
    void request_interrupt(InterruptType type);

    u8 ei_count;

private:
    void process_ei_count();
    bool if_get(u8 int_flag);

    Memory& mem;
    bool ime;
    u8 i_flags;        
    u8 i_enable;
};