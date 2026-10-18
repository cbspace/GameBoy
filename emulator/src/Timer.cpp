#include "Timer.h"
#include "Memory.h"

Timer::Timer(Memory& mem_in, Interrupt& ir_in) :
    mem(mem_in),
    ir(ir_in),
    internal_counter(0xABCC),
    tima(0),
    tma(0),
    tac(0),
    tima_overflow_cycles(0)
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
        case 0xFF04: // DIV
            // Writing any value resets the entire 16-bit internal counter
            internal_counter = 0;
            break;

        case 0xFF05: // TIMA
            // Writing to TIMA cancels any pending TMA reload
            if (tima_overflow_cycles > 0)
            {
                tima_overflow_cycles = 0;
            }
            tima = value;
            break;

        case 0xFF06: // TMA
            tma = value;
            // If TMA is written on the exact cycle of reload, TIMA takes the new value
            if (tima_overflow_cycles == 1)
            {
                tima = value;
            }
            break;

        case 0xFF07: // TAC
            tac = value & 0x07;
            break;
    }
}

void Timer::tick(u32 cycles)
{
    while (cycles > 0)
    {
        // Advance internal 16-bit divider
        u16 prev_counter = internal_counter;
        internal_counter++;

        // Determine which bit of the internal counter clocks TIMA
        // Mode 0: bit 9  (1024 T-cycles -> 4096 Hz)
        // Mode 1: bit 3  (16 T-cycles   -> 262144 Hz)
        // Mode 2: bit 5  (64 T-cycles   -> 65536 Hz)
        // Mode 3: bit 7  (256 T-cycles  -> 16384 Hz)
        static const int bit_shift[4] = { 9, 3, 5, 7 };
        int shift = bit_shift[tac & 0x03];

        bool timer_enabled = (tac & 0x04) != 0;
        bool prev_edge = timer_enabled && ((prev_counter >> shift) & 1);
        bool new_edge  = timer_enabled && ((internal_counter >> shift) & 1);

        // Falling edge detector: was 1, became 0
        if (prev_edge && !new_edge)
        {
            if (tima == 0xFF)
            {
                tima = tma;
                // Request Timer Interrupt (bit 2 of IF)
                u8 iflags = mem.get_byte(A_IFLAGS);
                mem.write_byte(A_IFLAGS, iflags | 0x04);
            }
            else
            {
                tima++;
            }
        }

        cycles--;
    }
}