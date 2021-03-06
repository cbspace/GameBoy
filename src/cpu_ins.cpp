#include "cpu.h"
#include "memory.h"
#include "clock.h"
#include "interrupt.h"

// Process a instruction read from ROM
void Cpu::process_instruction(uint8_t rom_byte)
{
    // Used for temporary 8-bit variable
    uint8_t tmp8_value;

    // Used for signed 8-bit variable
    int8_t signed8_imm;

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
            mem->write_byte(imm16_value,mem->reg_get(RA));
            clk->add_cycles(16);
            break;
        case 0xf2:  // Set register A to ($FF00 + register C)
            imm16_value = 0xff00 + mem->reg_get(RC);
            mem->reg_set(RA,mem->get_byte(imm16_value));
            clk->add_cycles(8);
            break;
        case 0xe2:  // Set byte at ($FF00 + register C) to register A
            imm16_value = 0xff00 + mem->reg_get(RC);
            mem->write_byte(imm16_value,mem->reg_get(RA));
            clk->add_cycles(8);
            break;
        case 0x3a:  // Set register A to value at (HL), decrement HL
            mem->reg_set(RA, mem->get_from_pointer(RHL));
            imm16_value = mem->reg_get16(RHL) - 1;
            mem->reg_set(RHL, imm16_value);
            clk->add_cycles(8);
            break;
        case 0x32:  // Set byte at (HL) to register A, decrement HL
            mem->set_from_pointer(RHL,mem->reg_get(RA));
            imm16_value = mem->reg_get16(RHL) - 1;
            mem->reg_set(RHL, imm16_value);
            clk->add_cycles(8);
            break;
        case 0x2a:  // Set register A to value at (HL), increment HL
            mem->reg_set(RA, mem->get_from_pointer(RHL));
            imm16_value = mem->reg_get16(RHL) + 1;
            mem->reg_set(RHL, imm16_value);
            clk->add_cycles(8);
            break;
        case 0x22:  // Set byte at (HL) to register A, increment HL
            mem->set_from_pointer(RHL,mem->reg_get(RA));
            imm16_value = mem->reg_get16(RHL) + 1;
            mem->reg_set(RHL, imm16_value);
            clk->add_cycles(8);
            break;
        case 0xe0:  // Set byte at ($FF00 + imm8 n) to register A
            imm16_value = 0xff00 + mem->fetch_byte();
            mem->write_byte(imm16_value,mem->reg_get(RA));
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
            mem->flag_update(ZF,0);
            mem->flag_update(NF,0);                        // NEED TO ADD H AND C FLAG OPERATIONS
            mem->reg_set(RF,tmp8_value);
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
        case 0xf5:  // Push AF register onto stack
            mem->stack_push(mem->reg_get16(RAF));
            clk->add_cycles(16);
            break;
        case 0xc5:  // Push BC register onto stack
            mem->stack_push(mem->reg_get16(RBC));
            clk->add_cycles(16);
            break;
        case 0xd5:  // Push DE register onto stack
            mem->stack_push(mem->reg_get16(RDE));
            clk->add_cycles(16);
            break;
        case 0xe5:  // Push HL register onto stack
            mem->stack_push(mem->reg_get16(RHL));
            clk->add_cycles(16);
            break;
        case 0xf1:  // Pop stack 16-bit value to AF register
            mem->reg_set(RAF,mem->stack_pop());
            clk->add_cycles(12);
            break;
        case 0xc1:  // Pop stack 16-bit value to BC register
            mem->reg_set(RBC,mem->stack_pop());
            clk->add_cycles(12);
            break;
        case 0xd1:  // Pop stack 16-bit value to DE register
            mem->reg_set(RDE,mem->stack_pop());
            clk->add_cycles(12);
            break;
        case 0xe1:  // Pop stack 16-bit value to HL register
            mem->reg_set(RHL,mem->stack_pop());
            clk->add_cycles(12);
            break;
        case 0x87:  // Set register A to reg A + reg A
            mem->reg_add(RA, mem->reg_get(RA));
            clk->add_cycles(4);
            break;
        case 0x80:  // Set register A to reg A + reg B
            mem->reg_add(RA, mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0x81:  // Set register A to reg A + reg C
            mem->reg_add(RA, mem->reg_get(RC));
            clk->add_cycles(4);
            break;
        case 0x82:  // Set register A to reg A + reg D
            mem->reg_add(RA, mem->reg_get(RD));
            clk->add_cycles(4);
            break;
        case 0x83:  // Set register A to reg A + reg E
            mem->reg_add(RA, mem->reg_get(RE));
            clk->add_cycles(4);
            break;
        case 0x84:  // Set register A to reg A + reg H
            mem->reg_add(RA, mem->reg_get(RH));
            clk->add_cycles(4);
            break;
        case 0x85:  // Set register A to reg A + reg L
            mem->reg_add(RA, mem->reg_get(RL));
            clk->add_cycles(4);
            break;
        case 0x86:  // Set register A to reg A + value at (HL)
            mem->reg_add(RA, mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0xc6:  // Set register A to reg A + imm8 n
            mem->reg_add(RA, mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x8f:  // Set register A to reg A + reg A + carry
            mem->reg_add(RA,mem->reg_get(RA),true);
            clk->add_cycles(4);
            break;
        case 0x88:  // Set register A to reg A + reg B + carry
            mem->reg_add(RA,mem->reg_get(RB),true);
            clk->add_cycles(4);
            break;
        case 0x89:  // Set register A to reg A + reg C + carry
            mem->reg_add(RA,mem->reg_get(RC),true);
            clk->add_cycles(4);
            break;
        case 0x8a:  // Set register A to reg A + reg D + carry
            mem->reg_add(RA,mem->reg_get(RD),true);
            clk->add_cycles(4);
            break;
        case 0x8b:  // Set register A to reg A + reg E + carry
            mem->reg_add(RA,mem->reg_get(RE),true);
            clk->add_cycles(4);
            break;
        case 0x8c:  // Set register A to reg A + reg H + carry
            mem->reg_add(RA,mem->reg_get(RH),true);
            clk->add_cycles(4);
            break;
        case 0x8d:  // Set register A to reg A + reg L + carry
            mem->reg_add(RA,mem->reg_get(RL),true);
            clk->add_cycles(4);
            break;
        case 0x8e:  // Set register A to reg A + (HL) + carry
            mem->reg_add(RA,mem->get_from_pointer(RHL),true);
            clk->add_cycles(8);
            break;
        case 0xce:  // Set register A to reg A + imm8 n + carry
            mem->reg_add(RA,mem->fetch_byte(),true);
            clk->add_cycles(8);
            break;
        case 0x97:  // Set register A to reg A minus reg A
            mem->reg_sub(RA,mem->reg_get(RA));
            clk->add_cycles(4);
            break;
        case 0x90:  // Set register A to reg A minus reg B
            mem->reg_sub(RA,mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0x91:  // Set register A to reg A minus reg C
            mem->reg_sub(RA,mem->reg_get(RC));
            clk->add_cycles(4);
            break;
        case 0x92:  // Set register A to reg A minus reg D
            mem->reg_sub(RA,mem->reg_get(RD));
            clk->add_cycles(4);
            break;
        case 0x93:  // Set register A to reg A minus reg E
            mem->reg_sub(RA,mem->reg_get(RE));
            clk->add_cycles(4);
            break;
        case 0x94:  // Set register A to reg A minus reg H
            mem->reg_sub(RA,mem->reg_get(RH));
            clk->add_cycles(4);
            break;
        case 0x95:  // Set register A to reg A minus reg L
            mem->reg_sub(RA,mem->reg_get(RL));
            clk->add_cycles(4);
            break;
        case 0x96:  // Set register A to reg A minus (HL)
            mem->reg_sub(RA,mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0xd6:  // Set register A to reg A minus imm8 n
            mem->reg_sub(RA,mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x9f:  // Set register A to reg A minus reg A - carry
            mem->reg_sub(RA,mem->reg_get(RA),true);
            clk->add_cycles(4);
            break;
        case 0x98:  // Set register A to reg A minus reg B - carry
            mem->reg_sub(RA,mem->reg_get(RB),true);
            clk->add_cycles(4);
            break;
        case 0x99:  // Set register A to reg A minus reg C - carry
            mem->reg_sub(RA,mem->reg_get(RC),true);
            clk->add_cycles(4);
            break;
        case 0x9a:  // Set register A to reg A minus reg D - carry
            mem->reg_sub(RA,mem->reg_get(RD),true);
            clk->add_cycles(4);
            break;
        case 0x9b:  // Set register A to reg A minus reg E - carry
            mem->reg_sub(RA,mem->reg_get(RE),true);
            clk->add_cycles(4);
            break;
        case 0x9c:  // Set register A to reg A minus reg H - carry
            mem->reg_sub(RA,mem->reg_get(RH),true);
            clk->add_cycles(4);
            break;
        case 0x9d:  // Set register A to reg A minus reg L - carry
            mem->reg_sub(RA,mem->reg_get(RL),true);
            clk->add_cycles(4);
            break;
        case 0x9e:  // Set register A to reg A minus (HL) - carry
            mem->reg_sub(RA,mem->get_from_pointer(RHL),true);
            clk->add_cycles(8);
            break;
        case 0xde:  // Set register A to reg A minus imm8 n - carry
            mem->reg_sub(RA,mem->fetch_byte(),true);
            clk->add_cycles(8);
            break;
        case 0xa7:  // Set register A to reg A AND reg A
            mem->reg_and(mem->reg_get(RA));
            clk->add_cycles(4);
            break;
        case 0xa0:  // Set register A to reg A AND reg B
            mem->reg_and(mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0xa1:  // Set register A to reg A AND reg C
            mem->reg_and(mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0xa2:  // Set register A to reg A AND reg D
            mem->reg_and(mem->reg_get(RD));
            clk->add_cycles(4);
            break;
        case 0xa3:  // Set register A to reg A AND reg E
            mem->reg_and(mem->reg_get(RE));
            clk->add_cycles(4);
            break;
        case 0xa4:  // Set register A to reg A AND reg H
            mem->reg_and(mem->reg_get(RH));
            clk->add_cycles(4);
            break;
        case 0xa5:  // Set register A to reg A AND reg L
            mem->reg_and(mem->reg_get(RL));
            clk->add_cycles(4);
            break;
        case 0xa6:  // Set register A to reg A AND (HL)
            mem->reg_and(mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0xe6:  // Set register A to reg A AND imm8 n
            mem->reg_and(mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0xb7:  // Set register A to reg A OR reg A
            mem->reg_or(mem->reg_get(RA));
            clk->add_cycles(4);
            break;
        case 0xb0:  // Set register A to reg A OR reg B
            mem->reg_or(mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0xb1:  // Set register A to reg A OR reg C
            mem->reg_or(mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0xb2:  // Set register A to reg A OR reg D
            mem->reg_or(mem->reg_get(RD));
            clk->add_cycles(4);
            break;
        case 0xb3:  // Set register A to reg A OR reg E
            mem->reg_or(mem->reg_get(RE));
            clk->add_cycles(4);
            break;
        case 0xb4:  // Set register A to reg A OR reg H
            mem->reg_or(mem->reg_get(RH));
            clk->add_cycles(4);
            break;
        case 0xb5:  // Set register A to reg A OR reg L
            mem->reg_or(mem->reg_get(RL));
            clk->add_cycles(4);
            break;
        case 0xb6:  // Set register A to reg A OR (HL)
            mem->reg_or(mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0xf6:  // Set register A to reg A OR imm8 n
            mem->reg_or(mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0xaf:  // Set register A to reg A XOR reg A
            mem->reg_xor(mem->reg_get(RA));
            clk->add_cycles(4);
            break;
        case 0xa8:  // Set register A to reg A XOR reg B
            mem->reg_xor(mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0xa9:  // Set register A to reg A XOR reg C
            mem->reg_xor(mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0xaa:  // Set register A to reg A XOR reg D
            mem->reg_xor(mem->reg_get(RD));
            clk->add_cycles(4);
            break;
        case 0xab:  // Set register A to reg A XOR reg E
            mem->reg_xor(mem->reg_get(RE));
            clk->add_cycles(4);
            break;
        case 0xac:  // Set register A to reg A XOR reg H
            mem->reg_xor(mem->reg_get(RH));
            clk->add_cycles(4);
            break;
        case 0xad:  // Set register A to reg A XOR reg L
            mem->reg_xor(mem->reg_get(RL));
            clk->add_cycles(4);
            break;
        case 0xae:  // Set register A to reg A XOR (HL)
            mem->reg_xor(mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0xee:  // Set register A to reg A XOR imm8 n
            mem->reg_xor(mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0xbf:  // Compare register A to register A and update flags
            mem->reg_compare(mem->reg_get(RA));
            clk->add_cycles(4);
            break;
        case 0xb8:  // Compare register A to register B and update flags
            mem->reg_compare(mem->reg_get(RB));
            clk->add_cycles(4);
            break;
        case 0xb9:  // Compare register A to register C and update flags
            mem->reg_compare(mem->reg_get(RC));
            clk->add_cycles(4);
            break;
        case 0xba:  // Compare register A to register D and update flags
            mem->reg_compare(mem->reg_get(RD));
            clk->add_cycles(4);
            break;
        case 0xbb:  // Compare register A to register E and update flags
            mem->reg_compare(mem->reg_get(RE));
            clk->add_cycles(4);
            break;
        case 0xbc:  // Compare register A to register F and update flags
            mem->reg_compare(mem->reg_get(RH));
            clk->add_cycles(4);
            break;
        case 0xbd:  // Compare register A to register L and update flags
            mem->reg_compare(mem->reg_get(RL));
            clk->add_cycles(4);
            break;
        case 0xbe:  // Compare register A to (HL) and update flags
            mem->reg_compare(mem->get_from_pointer(RHL));
            clk->add_cycles(8);
            break;
        case 0xfe:  // Compare register A to imm8 n and update flags
            mem->reg_compare(mem->fetch_byte());
            clk->add_cycles(8);
            break;
        case 0x3c:  // Increment register A by 1 and update flags
            mem->reg_inc(RA);
            clk->add_cycles(4);
            break;
        case 0x04:  // Increment register B by 1 and update flags
            mem->reg_inc(RB);
            clk->add_cycles(4);
            break;
        case 0x0c:  // Increment register C by 1 and update flags
            mem->reg_inc(RC);
            clk->add_cycles(4);
            break;
        case 0x14:  // Increment register D by 1 and update flags
            mem->reg_inc(RD);
            clk->add_cycles(4);
            break;
        case 0x1c:  // Increment register E by 1 and update flags
            mem->reg_inc(RE);
            clk->add_cycles(4);
            break;
        case 0x24:  // Increment register H by 1 and update flags
            mem->reg_inc(RH);
            clk->add_cycles(4);
            break;
        case 0x2c:  // Increment register L by 1 and update flags
            mem->reg_inc(RL);
            clk->add_cycles(4);
            break;
        case 0x34:  // Increment byte at (HL) by 1 and update flags
            mem->inc_from_pointer(RHL);
            clk->add_cycles(12);
            break;
        case 0x3d:  // Decrement register A by 1 and update flags
            mem->reg_dec(RA);
            clk->add_cycles(4);
            break;
        case 0x05:  // Decrement register B by 1 and update flags
            mem->reg_dec(RB);
            clk->add_cycles(4);
            break;
        case 0x0d:  // Decrement register C by 1 and update flags
            mem->reg_dec(RC);
            clk->add_cycles(4);
            break;
        case 0x15:  // Decrement register D by 1 and update flags
            mem->reg_dec(RD);
            clk->add_cycles(4);
            break;
        case 0x1d:  // Decrement register E by 1 and update flags
            mem->reg_dec(RE);
            clk->add_cycles(4);
            break;
        case 0x25:  // Decrement register H by 1 and update flags
            mem->reg_dec(RH);
            clk->add_cycles(4);
            break;
        case 0x2d:  // Decrement register L by 1 and update flags
            mem->reg_dec(RL);
            clk->add_cycles(4);
            break;
        case 0x35:  // Decrement byte at (HL) by 1 and update flags
            mem->dec_from_pointer(RHL);
            clk->add_cycles(12);
            break;
        case 0x09:  // Register HL is set to register HL + register BC, flags updated
            mem->reg_add16(RHL,mem->reg_get16(RBC));
            clk->add_cycles(8);
            break;
        case 0x19:  // Register HL is set to register HL + register DE, flags updated
            mem->reg_add16(RHL,mem->reg_get16(RDE));
            clk->add_cycles(8);
            break;
        case 0x29:  // Register HL is set to register HL + register HL, flags updated
            mem->reg_add16(RHL,mem->reg_get16(RHL));
            clk->add_cycles(8);
            break;
        case 0x39:  // Register HL is set to register HL + SP, flags updated
            mem->reg_add16(RHL,mem->get_sp());
            clk->add_cycles(8);
            break;
        case 0xe8:  // SP is set to SP + imm8 n, flags updated
            mem->sp_add(mem->fetch_byte());
            clk->add_cycles(16);
            break;
        case 0x03:  // Increment BC register by 1, flags not updated
            mem->reg_inc16(RBC);
            clk->add_cycles(8);
            break;
        case 0x13:  // Increment DE register by 1, flags not updated
            mem->reg_inc16(RDE);
            clk->add_cycles(8);
            break;
        case 0x23:  // Increment HL register by 1, flags not updated
            mem->reg_inc16(RHL);
            clk->add_cycles(8);
            break;
        case 0x33:  // Increment SP register by 1, flags not updated
            mem->inc_sp(1);
            clk->add_cycles(8);
            break;
        case 0x0b:  // Decrement BC register by 1, flags not updated
            mem->reg_dec16(RBC);
            clk->add_cycles(8);
            break;
        case 0x1b:  // Decrement DE register by 1, flags not updated
            mem->reg_dec16(RDE);
            clk->add_cycles(8);
            break;
        case 0x2b:  // Decrement HL register by 1, flags not updated
            mem->reg_dec16(RHL);
            clk->add_cycles(8);
            break;
        case 0x3b:  // Decrement SP register by 1, flags not updated
            mem->dec_sp(1);
            clk->add_cycles(8);
            break;
        case 0xcb:  // Process a CB prefixed instruction
            process_cb_instruction(mem->fetch_byte()); // Cycles are added as instructions are processed
            break;
        case 0x27:  // Decimal Adjust register A (DAA)
            mem->reg_daa();
            clk->add_cycles(4);
            break;
        case 0x2f:  // Complement A register and update flags
            tmp8_value = ~mem->reg_get(RA);
            mem->reg_set(RA, tmp8_value);
            mem->flag_update(NF,1);
            mem->flag_update(HF,1);
            clk->add_cycles(4);
            break;
        case 0x3f:  // Complement carry flag
            mem->flag_update(CF,!mem->flag_get(CF));
            mem->flag_update(NF,0);
            mem->flag_update(HF,0);
            clk->add_cycles(4);
            break;
        case 0x37:  // Set carry flag
            mem->flag_update(CF,1);
            mem->flag_update(NF,0);
            mem->flag_update(HF,0);
            clk->add_cycles(4);
            break;
        case 0x00:  // No operation (NOP)
            clk->add_cycles(4);
            break;
        case 0x76:  // Halt the CPU until an interrupt occurs (HALT)
            ir->cpu_halt();
            clk->add_cycles(4);
            break;
        case 0x10:  // Stop the CPU and LCD until a button is pressed (STOP)
            mem->fetch_byte(); // Byte should be 0x00
            ir->cpu_stop();
            clk->add_cycles(4);
            break;
        case 0xf3:  // Disable interrupts (DI)
            ir->disable_interrupts();
            clk->add_cycles(4);
            break;
        case 0xfb:  // Enable interrupts (EI) - happens after next instruction
            ir->ei_count = 1; // Set ei counter so that ei is effective after next instruction
            clk->add_cycles(4);
            break;
        case 0x07:  // Rotate contents of A register left and store bit 7 in CF, flags updated
            mem->reg_rla(true); // Carry = true (Rotate left with carry RLCA)
            clk->add_cycles(4);
            break;
        case 0x17:  // Rotate contents of A register left and store bit 7 in CF, flags updated
            mem->reg_rla(false); // Carry = false (Rotate left RLA)
            clk->add_cycles(4);
            break;
        case 0x0f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            mem->reg_rra(true); // Carry = true (Rotate right with carry RRCA)
            clk->add_cycles(4);
            break;
        case 0x1f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            mem->reg_rra(false); // Carry = false (Rotate right RRA)
            clk->add_cycles(4);
            break;
        case 0xc3:  // Jump to subroutine at nn
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xc2:  // Jump to subroutine at nn if ZF is reset
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            if (!mem->flag_get(ZF))
            {
                mem->set_pc(imm16_value);
                clk->add_cycles(16);
            }
            else
            {
                mem->inc_pc(1);
                clk->add_cycles(12);
            }
            break;
        case 0xca:  // Jump to subroutine at nn if ZF is set
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            if (mem->flag_get(ZF))
            {
                mem->set_pc(imm16_value);
                clk->add_cycles(16);
            }
            else
            {
                mem->inc_pc(1);
                clk->add_cycles(12);
            }
            break;
        case 0xd2:  // Jump to subroutine at nn if CF is reset
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            if (!mem->flag_get(CF))
            {
                mem->set_pc(imm16_value);
                clk->add_cycles(16);
            }
            else
            {
                mem->inc_pc(1);
                clk->add_cycles(12);
            }
            break;
        case 0xda:  // Jump to subroutine at nn if CF is set
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            if (mem->flag_get(CF))
            {
                mem->set_pc(imm16_value);
                clk->add_cycles(16);
            }
            else
            {
                mem->inc_pc(1);
                clk->add_cycles(12);
            }
            break;
        case 0xe9:  // Jump to address contained in HL
            mem->set_pc(mem->reg_get16(RHL));
            clk->add_cycles(4);
            break;
        case 0x18:  // Jump to address at PC + e (e = signed 8-bit immediate)
            signed8_imm = mem->fetch_byte();
            mem->jmp_n(signed8_imm);
            clk->add_cycles(12);
            break;
        case 0x20:  // Jump to address at PC + e if ZF is reset (e = signed 8-bit immediate)
            signed8_imm = mem->fetch_byte();
            if (!mem->flag_get(ZF))
            {
                mem->jmp_n(signed8_imm);
                clk->add_cycles(12);
            }
            else
            {
                //mem->inc_pc(1);
                clk->add_cycles(8);
            }
            break;
        case 0x28:  // Jump to address at PC + e if ZF is set (e = signed 8-bit immediate)
            signed8_imm = mem->fetch_byte();
            if (mem->flag_get(ZF))
            {
                mem->jmp_n(signed8_imm);
                clk->add_cycles(12);
            }
            else
            {
                //mem->inc_pc(1);
                clk->add_cycles(8);
            }
            break;
        case 0x30:  // Jump to address at PC + e if CF is reset (e = signed 8-bit immediate)
            signed8_imm = mem->fetch_byte();
            if (!mem->flag_get(CF))
            {
                mem->jmp_n(signed8_imm);
                clk->add_cycles(12);
            }
            else
            {
                //mem->inc_pc(1);
                clk->add_cycles(8);
            }
            break;
        case 0x38:  // Jump to address at PC + e if CF is set (e = signed 8-bit immediate)
            signed8_imm = mem->fetch_byte();
            if (mem->flag_get(CF))
            {
                mem->jmp_n(signed8_imm);
                clk->add_cycles(12);
            }
            else
            {
                //mem->inc_pc(1);
                clk->add_cycles(8);
            }
            break;
        case 0xcd:  // Push PC onto stack and jmp to nn (CALL)
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(24);
            break;
        case 0xc4:  // Push PC onto stack and jmp to nn (CALL) if ZF is reset
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            if (!mem->flag_get(ZF))
            {
                mem->pc_push();
                mem->set_pc(imm16_value);
                clk->add_cycles(24);
            }
            else
            {
                //mem->inc_pc(1);
                clk->add_cycles(12);
            }
            break;
        case 0xcc:  // Push PC onto stack and jmp to nn (CALL) if ZF is set
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            if (mem->flag_get(ZF))
            {
                mem->pc_push();
                mem->set_pc(imm16_value);
                clk->add_cycles(24);
            }
            else
            {
                //mem->inc_pc(1);
                clk->add_cycles(12);
            }
            break;
        case 0xd4:  // Push PC onto stack and jmp to nn (CALL) if CF is reset
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            if (!mem->flag_get(CF))
            {
                mem->pc_push();
                mem->set_pc(imm16_value);
                clk->add_cycles(24);
            }
            else
            {
                //mem->inc_pc(1);
                clk->add_cycles(12);
            }
            break;
        case 0xdc:  // Push PC onto stack and jmp to nn (CALL) if CF is set
            imm16_value = mem->fetch_byte();
            imm16_value += mem->fetch_byte() << 8;
            if (mem->flag_get(CF))
            {
                mem->pc_push();
                mem->set_pc(imm16_value);
                clk->add_cycles(24);
            }
            else
            {
                //mem->inc_pc(1);
                clk->add_cycles(12);
            }
            break;
        case 0xc7:  // Push PC onto stack and jmp to address 0x0000 + 0x00 (RST)
            imm16_value = 0x0000;
            mem->inc_pc(1);
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xcf:  // Push PC onto stack and jmp to address 0x0000 + 0x08 (RST)
            imm16_value = 0x0008;
            mem->inc_pc(1);
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xd7:  // Push PC onto stack and jmp to address 0x0000 + 0x10 (RST)
            imm16_value = 0x0010;
            mem->inc_pc(1);
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xdf:  // Push PC onto stack and jmp to address 0x0000 + 0x18 (RST)
            imm16_value = 0x0018;
            mem->inc_pc(1);
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xe7:  // Push PC onto stack and jmp to address 0x0000 + 0x20 (RST)
            imm16_value = 0x0020;
            mem->inc_pc(1);
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xef:  // Push PC onto stack and jmp to address 0x0000 + 0x28 (RST)
            imm16_value = 0x0028;
            mem->inc_pc(1);
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xf7:  // Push PC onto stack and jmp to address 0x0000 + 0x30 (RST)
            imm16_value = 0x0030;
            mem->inc_pc(1);
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xff:  // Push PC onto stack and jmp to address 0x0000 + 0x20 (RST)
            imm16_value = 0x0038;
            mem->inc_pc(1);
            mem->pc_push();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xc9:  // Pop 2 bytes from the stack and jump to that address (RET)
            imm16_value = mem->stack_pop();
            mem->set_pc(imm16_value);
            clk->add_cycles(16);
            break;
        case 0xc0:  // Pop 2 bytes from the stack and jump to that address if ZF is reset
            if (!mem->flag_get(ZF))
            {
                imm16_value = mem->stack_pop();
                mem->set_pc(imm16_value);
                clk->add_cycles(20);
            }
            else
            {
                mem->inc_pc(1);
                clk->add_cycles(8);
            }
            break;
        case 0xc8:  // Pop 2 bytes from the stack and jump to that address if ZF is set
            if (mem->flag_get(ZF))
            {
                imm16_value = mem->stack_pop();
                mem->set_pc(imm16_value);
                clk->add_cycles(20);
            }
            else
            {
                mem->inc_pc(1);
                clk->add_cycles(8);
            }
            break;
        case 0xd0:  // Pop 2 bytes from the stack and jump to that address if CF is reset
            if (!mem->flag_get(CF))
            {
                imm16_value = mem->stack_pop();
                mem->set_pc(imm16_value);
                clk->add_cycles(20);
            }
            else
            {
                mem->inc_pc(1);
                clk->add_cycles(8);
            }
            break;
        case 0xd8:  // Pop 2 bytes from the stack and jump to that address if CF is set
            if (mem->flag_get(CF))
            {
                imm16_value = mem->stack_pop();
                mem->set_pc(imm16_value);
                clk->add_cycles(20);
            }
            else
            {
                mem->inc_pc(1);
                clk->add_cycles(8);
            }
            break;
        case 0xd9:  // Pop 2 bytes from the stack, jump to that address and enable interrupts (RETI)
            imm16_value = mem->stack_pop();
            mem->set_pc(imm16_value);
            // Enable interrupts
            clk->add_cycles(16);
            break;
    }
}

