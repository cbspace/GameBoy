#pragma once
#include "lib/Types.h"
#include "interrupt.h"

class Memory;

class Timer
{
public:
    Timer(Memory& mem_in, Interrupt& ir_in);

    // Call this after every CPU instruction with the elapsed T-cycles (4, 8, 12, etc.)
    void tick(u32 cycles);

    // Memory read/write intercepts for 0xFF04 - 0xFF07
    u8 read_byte(u16 address) const;
    void write_byte(u16 address, u8 value);

private:
    Memory& mem;
    Interrupt& ir;

    u16 internal_counter = 0; // 16-bit hardware counter (DIV is internal_counter >> 8)
    u8 tima = 0;              // 0xFF05
    u8 tma = 0;               // 0xFF06
    u8 tac = 0;               // 0xFF07

    // Overflow / reload state
    bool tima_overflow_delay = false;
};