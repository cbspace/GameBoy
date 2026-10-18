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
    auto get_edge = [this]() -> bool {
        bool enabled = (tac & 0x04) != 0;
        int bit_pos = 9;
        switch (tac & 0x03)
        {
            case 0: bit_pos = 9; break;
            case 1: bit_pos = 3; break;
            case 2: bit_pos = 5; break;
            case 3: bit_pos = 7; break;
        }
        return enabled && ((internal_counter & (1 << bit_pos)) != 0);
    };

    switch (address)
    {
        case 0xFF04: // DIV
        {
            bool before = get_edge();
            internal_counter = 0; // Reset entire 16-bit counter
            bool after = get_edge();

            // Glitch: resetting counter dropped bit from 1 to 0
            if (before && !after)
            {
                tima++;
                if (tima == 0x00)
                {
                    tima_overflow_cycles = 4;
                }
            }
            break;
        }

        case 0xFF05: // TIMA
            // If software writes to TIMA during the 4-cycle overflow delay,
            // the write cancels the TMA reload!
            if (tima_overflow_cycles > 0)
            {
                tima_overflow_cycles = 0;
            }
            tima = value;
            break;

        case 0xFF06: // TMA
            tma = value;
            // If TMA is written during the overflow delay, the new value gets reloaded
            if (tima_overflow_cycles == 1)
            {
                tima = value;
            }
            break;

        case 0xFF07: // TAC
        {
            bool before = get_edge();
            tac = value & 0x07;
            bool after = get_edge();

            // Changing mode or disabling timer dropped the signal
            if (before && !after)
            {
                tima++;
                if (tima == 0x00)
                {
                    tima_overflow_cycles = 4;
                }
            }
            break;
        }
    }
}

void Timer::tick(u32 cycles)
{
    for (u32 i = 0; i < cycles; ++i)
    {
        // 1. Multiplexer bit check
        bool timer_enabled = (tac & 0x04) != 0;
        int bit_pos = 9;
        switch (tac & 0x03)
        {
            case 0: bit_pos = 9; break;
            case 1: bit_pos = 3; break;
            case 2: bit_pos = 5; break;
            case 3: bit_pos = 7; break;
        }

        bool bit_before = timer_enabled && ((internal_counter & (1 << bit_pos)) != 0);

        internal_counter++;

        bool bit_after = timer_enabled && ((internal_counter & (1 << bit_pos)) != 0);

        // 2. Falling edge detection
        if (bit_before && !bit_after)
        {
            tima++;
            if (tima == 0x00)
            {
                // Delay reload by 4 T-cycles
                tima_overflow_cycles = 4;
            }
        }

        // 3. Process delay and reload
        if (tima_overflow_cycles > 0)
        {
            tima_overflow_cycles--;
            if (tima_overflow_cycles == 0)
            {
                tima = tma;
                ir.request_interrupt(InterruptType::Timer);
            }
        }
    }
}