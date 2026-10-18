#include "interrupt.h"
#include "Memory.h"
#include "DebugLogger.h"
#include <iostream>

using namespace std;

Interrupt::Interrupt(Memory& mem_in) :
    mem(mem_in)
{
    // Hardware initializes with IME disabled (set to false)
    ime = false;
    ei_count = 0;
    i_flags = 0;
    i_enable = 0;
}

void Interrupt::request_interrupt(InterruptType type)
{
    u8 mask = 0;
    switch (type)
    {
        case InterruptType::VBlank:  mask = I_VBLANK;  break;
        case InterruptType::LCDStat: mask = I_LCDSTAT; break;
        case InterruptType::Timer:   mask = I_TIMER;   break;
        case InterruptType::Serial:  mask = I_SERIAL;  break;
        case InterruptType::Joypad:  mask = I_JOYPAD;  break;
    }
    if_update(mask, true);
}

u32 Interrupt::check_interrupts()
{
    // Always fetch fresh flags directly from memory
    i_flags = mem.get_byte(A_IFLAGS);
    i_enable = mem.get_byte(A_IENABLE);
    u8 i_current = i_flags & i_enable & 0x1F; // Top 3 bits are unused/always 1

    if (ime && i_current)
    {
        // Log the exact vector event
        DebugLogger::instance().log_step(
            mem.get_pc(), 0x00, /* placeholder */
            mem.reg_get16(Register16Bit::AF),
            mem.reg_get16(Register16Bit::BC),
            mem.reg_get16(Register16Bit::DE),
            mem.reg_get16(Register16Bit::HL),
            mem.get_sp(),
            mem.get_byte(0xFF04),
            mem.get_byte(0xFF05),
            mem.get_byte(0xFF06),
            mem.get_byte(0xFF07),
            i_flags, i_enable, ime, 20
        );

        ime = false;
        mem.pc_push();

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

        return 20; // 5 M-cycles (20 T-states) dispatch overhead
    }

    return 0;
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
    if (ei_count > 0)
    {
        ei_count--;
        if (ei_count == 0)
        {
            ime = true;
        }
    }
}

void Interrupt::set_ime(bool ime_val) { ime=ime_val; }
bool Interrupt::get_ime() { return ime; }
void Interrupt::disable_interrupts() { ime = false; ei_count = 0; }
void Interrupt::enable_interrupts() { ei_count = 2; }