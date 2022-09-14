#include "cpu.h"
#include "memory.h"
#include "clock.h"
#include "interrupt.h"
#include "emudebug.h"
#include <SDL2/SDL.h>

Cpu::Cpu(Memory& mem_in, Clock& clk_in, Interrupt& ir_in, Emudebug& edb_in) :
    mem(mem_in),
    clk(clk_in),
    ir(ir_in),
    edb(edb_in),
    byte_in(0x00)
{}

void Cpu::cycle()
{
    uint8_t byte_in;

    if (!ir.get_halt() && !ir.get_stop())
    {
        byte_in = mem.fetch_byte();
        process_instruction(byte_in);
    }
    else if (ir.get_halt())
    {
        clk.add_cycles(4);
    }
}