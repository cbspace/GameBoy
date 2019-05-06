#include "cpu.h"
#include "memory.h"

// Constructor
Cpu::Cpu()
{
    mem = NULL;
}

// Constructor
void Cpu::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

// Perform a single CPU cycle
void Cpu::cycle()
{
    // Fetch byte for execution
    byte_in = mem->get_current_byte();
    printf("byte %0.2X\n", byte_in);

    // Increment the program counter
    mem->inc_pc(1);
}
