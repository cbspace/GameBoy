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
    uint8_t byte_in;

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
        case 0x06:  // Load immediate to B register

            // Get n value and store in B reg
            mem->reg_set( RB, mem->fetch_byte() );

            // Add clock cycles
            clk->add_cycles(8);

            break;
        case 0x0e:  // Load immediate to C register

            // Get n value and store in C reg
            mem->reg_set( RC, mem->fetch_byte() );

            // Add clock cycles
            clk->add_cycles(8);
            break;
        case 0x16:  // Load immediate to D register

            // Get n value and store in D reg
            mem->reg_set( RD, mem->fetch_byte() );

            // Add clock cycles
            clk->add_cycles(8);
            break;
        case 0x1e:  // Load immediate to E register

            // Get n value and store in E reg
            mem->reg_set( RE, mem->fetch_byte() );

            // Add clock cycles
            clk->add_cycles(8);
            break;
        case 0x26:  // Load immediate to H register

            // Get n value and store in H reg
            mem->reg_set( RH, mem->fetch_byte() );

            // Add clock cycles
            clk->add_cycles(8);
            break;
        case 0x2e:  // Load immediate to L register

            // Get n value and store in L reg
            mem->reg_set( RL, mem->fetch_byte() );

            // Add clock cycles
            clk->add_cycles(8);
            break;
        case 0x7f:  // Set register A to register A

            // Set register A to value of register A
            mem->reg_set( RA, mem->reg_get(RA) );

            // Add clock cycles
            clk->add_cycles(4);
            break;
        case 0x78:  // Set register A to register B

            // Set register A to value of register B
            mem->reg_set( RA, mem->reg_get(RB) );

            // Add clock cycles
            clk->add_cycles(4);
            break;
        case 0x79:  // Set register A to register C

            // Set register A to value of register C
            mem->reg_set( RA, mem->reg_get(RC) );

            // Add clock cycles
            clk->add_cycles(4);
            break;
        case 0x7a:  // Set register A to register D

            // Set register A to value of register D
            mem->reg_set( RA, mem->reg_get(RD) );

            // Add clock cycles
            clk->add_cycles(4);
            break;
        case 0x7b:  // Set register A to register E

            // Set register A to value of register E
            mem->reg_set( RA, mem->reg_get(RE) );

            // Add clock cycles
            clk->add_cycles(4);
            break;
        case 0x7c:  // Set register A to register H

            // Set register A to value of register H
            mem->reg_set( RA, mem->reg_get(RH) );

            // Add clock cycles
            clk->add_cycles(4);
            break;
        case 0x7d:  // Set register A to register L

            // Set register A to value of register L
            mem->reg_set( RA, mem->reg_get(RL) );

            // Add clock cycles
            clk->add_cycles(4);
            break;
        case 0x7e:  // Set register A to value at (HL)

            // Set register A to value of at (HL)
            mem->reg_set( RA, mem->read_byte(mem->reg_get16(RHL)) );

            // Add clock cycles
            clk->add_cycles(8);
            break;
    }

    // Increment the program counter(temp)
    mem->inc_pc(1);

    // Add clock cycles (temp so we don't get stuck)
    clk->add_cycles(4);
}
