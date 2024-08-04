#include "joypad.h"

Joypad::Joypad(Memory& mem_in, Interrupt& ir_in) : 
    mem(mem_in),
    ir(ir_in) 
{}

u8 Joypad::key_down()
{
    u8 quit_signal = 0;

    return quit_signal;
}