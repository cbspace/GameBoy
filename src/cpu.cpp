#include "cpu.h"
#include "memory.h"
#include "clock.h"

// Constructor
Cpu::Cpu()
{
    mem = NULL;
}

// Set pointer used to access memory object
void Cpu::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

// Set pointer used to access memory object
void Cpu::attach_clock(Clock* clock_in)
{
    clk = clock_in;
}

// Perform a single CPU cycle
void Cpu::cycle()
{
    // Fetch byte for execution
    //byte_in = mem->read_byte(mem->pc);
    //printf("byte %0.2X\n", byte_in);

    // Increment the program counter
    //mem->inc_pc(1);

    clk->add_cycles(4);
}
