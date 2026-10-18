#include "Timer.h"
#include "Memory.h"

Timer::Timer(Memory& mem_in, Interrupt& ir_in) :
    mem(mem_in),
    ir(ir_in),
    internal_counter(0xABCC) // Typical boot value (upper byte gives DIV ~ 0xAB)
{}

u8 Timer::read_byte(u16 address) const
{
    switch (address)
    {
        case 0xFF04: return static_cast<u8>(internal_counter >> 8);
        case 0xFF05: return tima;
        case 0xFF06: return tma;
        case 0xFF07: return tac | 0xF8; // Upper 5 bits always read 1
        default:     return 0xFF;
    }
}

void Timer::write_byte(u16 address, u8 value)
{
    switch (address)
    {
        case 0xFF04:
            // Writing any byte to DIV resets the entire internal counter to 0!
            internal_counter = 0;
            break;
        case 0xFF05:
            tima = value;
            break;
        case 0xFF06:
            tma = value;
            break;
        case 0xFF07:
            tac = value & 0x07;
            break;
    }
}

void Timer::tick(u32 cycles)
{
    // Step forward cycle by cycle (or in chunks)
    for (u32 i = 0; i < cycles; ++i)
    {
        // Handle the 1-cycle TIMA reload delay if an overflow occurred
        if (tima_overflow_delay)
        {
            tima = tma; // Reload from TMA
            ir.request_interrupt(InterruptType::Timer); // Bit 2 of 0xFF0F (IF)
            tima_overflow_delay = false;
        }

        // Determine the previous state of the selected multiplexer bit
        bool timer_enabled = (tac & 0x04) != 0;
        
        auto get_multiplexer_bit = [this]() -> bool {
            switch (tac & 0x03)
            {
                case 0: return (internal_counter & (1 << 9)) != 0; // 4096 Hz
                case 1: return (internal_counter & (1 << 3)) != 0; // 262144 Hz
                case 2: return (internal_counter & (1 << 5)) != 0; // 65536 Hz
                case 3: return (internal_counter & (1 << 7)) != 0; // 16384 Hz
                default: return false;
            }
        };

        bool bit_before = timer_enabled && get_multiplexer_bit();

        internal_counter++;

        bool bit_after = timer_enabled && get_multiplexer_bit();

        // Falling edge: transitioned from 1 -> 0
        if (bit_before && !bit_after)
        {
            tima++;
            if (tima == 0x00) // 8-bit overflow (was 0xFF, now 0x00)
            {
                // In hardware, TIMA stays 0x00 for 4 T-cycles before reloading TMA and setting IF
                tima_overflow_delay = true;
            }
        }
    }
}