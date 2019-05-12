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
    // Used for temporary 8-bit variable
    uint8_t tmp8_value;

    // Used for instructions that use 16-bit immediate value
    uint16_t imm16_value;

    // switch statement to match opcode and perform operations
    switch(rom_byte)
    {
        case 0x3e:  // Load immediate value n to A register
            mem->reg_set(RA, mem->fetch_byte());
            clk->add_cycles(8);
            break;
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
            mem->reg_set(RA, mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0x47:  // Set register B to register A
            mem->reg_copy(RB, RA);
            clk->add_cycles(4);
            break;
        case 0x40:  // Set register B to register B
            mem->reg_copy(RB, RB);
            clk->add_cycles(4);
            break;
        case 0x41:  // Set register B to register C
            mem->reg_copy(RB, RC);
            clk->add_cycles(4);
            break;
        case 0x42:  // Set register B to register D
            mem->reg_copy(RB, RD);
            clk->add_cycles(4);
            break;
        case 0x43:  // Set register B to register E
            mem->reg_copy(RB, RE);
            clk->add_cycles(4);
            break;
        case 0x44:  // Set register B to register H
            mem->reg_copy(RB, RH);
            clk->add_cycles(4);
            break;
        case 0x45:  // Set register B to register L
            mem->reg_copy(RB, RL);
            clk->add_cycles(4);
            break;
        case 0x46:  // Set register B to value at (HL)
            mem->reg_set(RB, mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0x4f:  // Set register C to register A
            mem->reg_copy(RC, RA);
            clk->add_cycles(4);
            break;
        case 0x48:  // Set register C to register B
            mem->reg_copy(RC, RB);
            clk->add_cycles(4);
            break;
        case 0x49:  // Set register C to register C
            mem->reg_copy(RC, RC);
            clk->add_cycles(4);
            break;
        case 0x4a:  // Set register C to register D
            mem->reg_copy(RC, RD);
            clk->add_cycles(4);
            break;
        case 0x4b:  // Set register C to register E
            mem->reg_copy(RC, RE);
            clk->add_cycles(4);
            break;
        case 0x4c:  // Set register C to register H
            mem->reg_copy(RC, RH);
            clk->add_cycles(4);
            break;
        case 0x4d:  // Set register C to register L
            mem->reg_copy(RC, RL);
            clk->add_cycles(4);
            break;
        case 0x4e:  // Set register C to value at (HL)
            mem->reg_set(RC, mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0x57:  // Set register D to register A
            mem->reg_copy(RD, RA);
            clk->add_cycles(4);
            break;
        case 0x50:  // Set register D to register B
            mem->reg_copy(RD, RD);
            clk->add_cycles(4);
            break;
        case 0x51:  // Set register D to register C
            mem->reg_copy(RD, RC);
            clk->add_cycles(4);
            break;
        case 0x52:  // Set register D to register D
            mem->reg_copy(RD, RD);
            clk->add_cycles(4);
            break;
        case 0x53:  // Set register D to register E
            mem->reg_copy(RD, RE);
            clk->add_cycles(4);
            break;
        case 0x54:  // Set register D to register H
            mem->reg_copy(RD, RH);
            clk->add_cycles(4);
            break;
        case 0x55:  // Set register D to register L
            mem->reg_copy(RD, RL);
            clk->add_cycles(4);
            break;
        case 0x56:  // Set register D to value at (HL)
            mem->reg_set(RD, mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0x5f:  // Set register E to register A
            mem->reg_copy(RE, RE);
            clk->add_cycles(4);
            break;
        case 0x58:  // Set register E to register B
            mem->reg_copy(RE, RE);
            clk->add_cycles(4);
            break;
        case 0x59:  // Set register E to register C
            mem->reg_copy(RE, RC);
            clk->add_cycles(4);
            break;
        case 0x5a:  // Set register E to register D
            mem->reg_copy(RE, RD);
            clk->add_cycles(4);
            break;
        case 0x5b:  // Set register E to register E
            mem->reg_copy(RE, RE);
            clk->add_cycles(4);
            break;
        case 0x5c:  // Set register E to register H
            mem->reg_copy(RE, RH);
            clk->add_cycles(4);
            break;
        case 0x5d:  // Set register E to register L
            mem->reg_copy(RE, RL);
            clk->add_cycles(4);
            break;
        case 0x5e:  // Set register E to value at (HL)
            mem->reg_set(RE, mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0x67:  // Set register H to register A
            mem->reg_copy(RH, RA);
            clk->add_cycles(4);
            break;
        case 0x60:  // Set register H to register B
            mem->reg_copy(RH, RH);
            clk->add_cycles(4);
            break;
        case 0x61:  // Set register H to register C
            mem->reg_copy(RH, RC);
            clk->add_cycles(4);
            break;
        case 0x62:  // Set register H to register D
            mem->reg_copy(RH, RD);
            clk->add_cycles(4);
            break;
        case 0x63:  // Set register H to register E
            mem->reg_copy(RH, RE);
            clk->add_cycles(4);
            break;
        case 0x64:  // Set register H to register H
            mem->reg_copy(RH, RH);
            clk->add_cycles(4);
            break;
        case 0x65:  // Set register H to register L
            mem->reg_copy(RH, RL);
            clk->add_cycles(4);
            break;
        case 0x66:  // Set register H to value at (HL)
            mem->reg_set(RH, mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0x6f:  // Set register L to register A
            mem->reg_copy(RL, RA);
            clk->add_cycles(4);
            break;
        case 0x68:  // Set register L to register B
            mem->reg_copy(RL, RL);
            clk->add_cycles(4);
            break;
        case 0x69:  // Set register L to register C
            mem->reg_copy(RL, RC);
            clk->add_cycles(4);
            break;
        case 0x6a:  // Set register L to register D
            mem->reg_copy(RL, RD);
            clk->add_cycles(4);
            break;
        case 0x6b:  // Set register L to register E
            mem->reg_copy(RL, RE);
            clk->add_cycles(4);
            break;
        case 0x6c:  // Set register L to register H
            mem->reg_copy(RL, RH);
            clk->add_cycles(4);
            break;
        case 0x6d:  // Set register L to register L
            mem->reg_copy(RL, RL);
            clk->add_cycles(4);
            break;
        case 0x6e:  // Set register L to value at (HL)
            mem->reg_set(RL, mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0x70:  // Set byte at (HL) to register B
            mem->set_from_pointer(RHL,mem->reg_get(RB));
            clk->add_cycles(8);
            break;
        case 0x71:  // Set byte at (HL) to register C
            mem->set_from_pointer(RHL,mem->reg_get(RC));
            clk->add_cycles(8);
            break;
        case 0x72:  // Set byte at (HL) to register D
            mem->set_from_pointer(RHL,mem->reg_get(RD));
            clk->add_cycles(8);
            break;
        case 0x73:  // Set byte at (HL) to register E
            mem->set_from_pointer(RHL,mem->reg_get(RE));
            clk->add_cycles(8);
            break;
        case 0x74:  // Set byte at (HL) to register H
            mem->set_from_pointer(RHL,mem->reg_get(RH));
            clk->add_cycles(8);
            break;
        case 0x75:  // Set byte at (HL) to register L
            mem->set_from_pointer(RHL,mem->reg_get(RL));
            clk->add_cycles(8);
            break;
        case 0x36:  // Set byte at (HL) to immediate byte n
            mem->set_from_pointer(RHL,mem->fetch_byte());
            clk->add_cycles(12);
            break;
        case 0x0a:  // Set register A to value at (BC)
            mem->reg_set(RA, mem->get_from_pointer(RBC));
            clk->add_cycles(8);
            break;
        case 0x1a:  // Set register A to value at (DE)
            mem->reg_set(RA, mem->get_from_pointer(RDE));
            clk->add_cycles(8);
            break;
        case 0xfa:  // Set register A to immediate value at (nn)
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            mem->reg_set(RA,mem->get_byte(imm16_value));
            clk->add_cycles(16);
            break;
        case 0x02:  // Set byte at (BC) to register A
            mem->set_from_pointer(RBC,mem->reg_get(RA));
            clk->add_cycles(8);
            break;
        case 0x12:  // Set byte at (DE) to register A
            mem->set_from_pointer(RDE,mem->reg_get(RA));
            clk->add_cycles(8);
            break;
        case 0x77:  // Set byte at (HL) to register A
            mem->set_from_pointer(RHL,mem->reg_get(RA));
            clk->add_cycles(8);
            break;
        case 0xea:  // Set byte at (nn) to register A
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            mem->write_byte(imm16_value,RA);
            clk->add_cycles(16);
            break;
        case 0xf2:  // Set register A to ($FF00 + register C)
            imm16_value = 0xff00 + mem->reg_get(RC);
            mem->reg_set(RA,mem->get_byte(imm16_value));
            clk->add_cycles(8);
            break;
        case 0xe2:  // Set byte at ($FF00 + register C) to register A
            imm16_value = 0xff00 + mem->reg_get(RC);
            mem->write_byte(imm16_value,RA);
            clk->add_cycles(8);
            break;
        case 0x3a:  // Set register A to value at (HL), decrement HL
            mem->reg_set(RA, mem->get_from_pointer(RHL));
            mem->reg_dec(RHL);
            clk->add_cycles(8);
            break;
        case 0x32:  // Set byte at (HL) to register A, decrement HL
            mem->set_from_pointer(RHL,mem->reg_get(RA));
            mem->reg_dec(RHL);
            clk->add_cycles(8);
            break;
        case 0x2a:  // Set register A to value at (HL), increment HL
            mem->reg_set(RA, mem->get_from_pointer(RHL));
            mem->reg_inc(RHL);
            clk->add_cycles(8);
            break;
        case 0x22:  // Set byte at (HL) to register A, increment HL
            mem->set_from_pointer(RHL,mem->reg_get(RA));
            mem->reg_inc(RHL);
            clk->add_cycles(8);
            break;
        case 0xe0:  // Set byte at ($FF00 + imm8 n) to register A
            imm16_value = 0xff00 + mem->fetch_byte();
            mem->write_byte(imm16_value,RA);
            clk->add_cycles(12);
            break;
        case 0xf0:  // Set register A to ($FF00 + imm8 n)
            imm16_value = 0xff00 + mem->fetch_byte();
            mem->reg_set(RA,mem->get_byte(imm16_value));
            clk->add_cycles(8);
            break;
        case 0x01:  // Set register BC to 16 bit immediate value nn
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            mem->reg_set(RBC,imm16_value);
            clk->add_cycles(12);
            break;
        case 0x11:  // Set register DE to 16 bit immediate value nn
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            mem->reg_set(RDE,imm16_value);
            clk->add_cycles(12);
            break;
        case 0x21:  // Set register HL to 16 bit immediate value nn
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            mem->reg_set(RHL,imm16_value);
            clk->add_cycles(12);
            break;
        case 0x31:  // Set SP to 16 bit immediate value nn
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            mem->set_sp(imm16_value);
            clk->add_cycles(12);
            break;
        case 0xf9:  // Set SP to value in HL
            mem->set_sp(mem->reg_get16(RHL));
            clk->add_cycles(8);
            break;
        case 0xf8:  // Set HL to SP + imm8 n
            imm16_value = mem->get_sp() + mem->fetch_byte();
            tmp8_value = mem->get_flags() & (H_FLAG + C_FLAG);
            mem->reg_set(RF,tmp8_value);
            mem->reg_set(RHL,imm16_value);
            clk->add_cycles(12);
            break;
        case 0x08:  // Set bytes at (nn) to SP value
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            // sp low byte
            tmp8_value = mem->get_sp() & 0xff;
            mem->write_byte(imm16_value,tmp8_value);
            // sp high byte
            tmp8_value = mem->get_sp() >> 8;
            mem->write_byte(imm16_value + 1,tmp8_value);
            clk->add_cycles(20);
            break;
    }

    // Increment the program counter(temp)
    mem->inc_pc(1);

    // Add clock cycles (temp so we don't get stuck)
    clk->add_cycles(4);
}
