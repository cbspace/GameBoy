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
    // Byte read from ROM
    uint8_t ebyte_in;

    // Fetch byte for execution
    byte_in = mem->fetch_byte();

    // Process the byte
    process_instruction(byte_in);
}

// Process a instruction read from ROM
void Cpu::process_instruction(uint8_t rom_byte)
{
    // switch statement to match opcode and perform operations
    switch(rom_byte)
    {
        case 0x06:  // Load immediate value n to B register
            mem->reg_set(RB, mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x0e:  // Load immediate value n to C register
            mem->reg_set(RC, mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x16:  // Load immediate value n to D register
            mem->reg_set(RD, mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x1e:  // Load immediate value n to E register
            mem->reg_set(RE, mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x26:  // Load immediate value n to H register
            mem->reg_set(RH, mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x2e:  // Load immediate value n to L register
            mem->reg_set(RL, mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x7f:  // Set register A to register A
            mem->reg_copy(RA, RA);
            clk->add_cycles(4);
            break;
        case 0x78:  // Set register A to register B
            mem->reg_copy(RA, RB);
            clk->add_cycles(4);
            break;
        case 0x79:  // Set register A to register C
            mem->reg_copy(RA, RC);
            clk->add_cycles(4);
            break;
        case 0x7a:  // Set register A to register D
            mem->reg_copy(RA, RD);
            clk->add_cycles(4);
            break;
        case 0x7b:  // Set register A to register E
            mem->reg_copy(RA, RE);
            clk->add_cycles(4);
            break;
        case 0x7c:  // Set register A to register H
            mem->reg_copy(RA, RH);
            clk->add_cycles(4);
            break;
        case 0x7d:  // Set register A to register L
            mem->reg_copy(RA, RL);
            clk->add_cycles(4);
            break;
        case 0x7e:  // Set register A to value at (HL)
            mem->reg_copy(RA, VHL);
            clk->add_cycles(8);
            break;
        case 0x40:  // Set register B to register B
            mem->reg_copy(RB, RB);
            clk->add_cycles(4);
            break;
    }

    // Increment the program counter(temp)
    mem->inc_pc(1);

    // Add clock cycles (temp so we don't get stuck)
    clk->add_cycles(4);
}
