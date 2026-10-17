#include "interrupt.h"
#include "Memory.h"
#include "Clock.h"
#include <iostream>

using namespace std;

Interrupt::Interrupt(Memory& mem_in) :
    mem(mem_in)
{
    ime = true;            // Hardware initializes with IME disabled
    ei_count = 0;
    i_flags = 0;
    i_enable = 0;
}

void Interrupt::check_interrupts()
{
    // Always fetch fresh flags directly from memory
    i_flags = mem.get_byte(A_IFLAGS);
    i_enable = mem.get_byte(A_IENABLE);
    u8 i_current = i_flags & i_enable & 0x1F; // Top 3 bits are unused/always 1

    if (ime && i_current)
    {
        ime = false;
        mem.pc_push();
        // clk.add_cycles(20); // 5 M-cycles (20 T-states) interrupt dispatch delay

        if (i_current & I_VBLANK)
        {
            if_update(I_VBLANK, false);
            mem.set_pc(IV_VBLANK); // 0x0040
        }
        else if (i_current & I_LCDSTAT)
        {
            if_update(I_LCDSTAT, false);
            mem.set_pc(IV_LCDSTAT); // 0x0048
        }
        else if (i_current & I_TIMER)
        {
            if_update(I_TIMER, false);
            mem.set_pc(IV_TIMER); // 0x0050
        }
        else if (i_current & I_SERIAL)
        {
            if_update(I_SERIAL, false);
            mem.set_pc(IV_SERIAL); // 0x0058
        }
        else if (i_current & I_JOYPAD)
        {
            if_update(I_JOYPAD, false);
            mem.set_pc(IV_JOYPAD); // 0x0060
        }
    }

    if (ei_count > 0)
    {
        process_ei_count();
    }
}

void Interrupt::if_update(u8 int_flag, bool flg_val)
{
    u8 current_if = mem.get_byte(A_IFLAGS);
    if (flg_val)
    {
        current_if |= int_flag;
    }
    else
    {
        current_if &= ~int_flag;
    }
    mem.write_byte(A_IFLAGS, current_if | 0xE0); // Upper 3 bits are always 1 on DMG
}

void Interrupt::process_ei_count()
{
    // EI takes effect after the instruction following EI
    if (ei_count == 1)
    {
        ei_count = 2;
    }
    else if (ei_count == 2)
    {
        ei_count = 0;
        ime = true;
    }
}

bool Interrupt::get_ime() { return ime; }
void Interrupt::disable_interrupts() { ime = false; ei_count = 0; }
void Interrupt::enable_interrupts() { ei_count = 1; }