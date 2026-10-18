#include "cpu.h"
#include "DebugLogger.h"
#include "Timer.h"

Cpu::Cpu(Memory& mem_in, Interrupt& ir_in, Timer& timer_in) :
    mem(mem_in),
    ir(ir_in),
    byte_in(0x00),
    halt_flag(false),
    stop_flag(false),
    timer(timer_in) //for debug
{}

u32 Cpu::cycle()
{
    ticks = 0;

    // 1. Check if a pending interrupt wakes the CPU from HALT
    if (halt_flag)
    {
        u8 i_flags = mem.get_byte(A_IFLAGS);
        u8 i_enable = mem.get_byte(A_IENABLE);

        if ((i_flags & i_enable & 0x1F) != 0) 
        {
            halt_flag = false; // Wake up
        } 
        else 
        {
            add_ticks(4); // HALT consumes 1 M-cycle while waiting
            return ticks;
        }
    }

    // 2. Execute single instruction if not stopped
    if (!stop_flag)
    {
        u16 current_pc = mem.get_pc();
        u8 opcode;

        if (current_pc == 0xC09C) {
            printf("LDH at C09C reading offset 0x%02X (address 0xFF%02X)\n", 
            mem.get_byte(current_pc + 1), mem.get_byte(current_pc + 1));
        }
        
        if (halt_bug_flag)
        {
            // HALT bug: PC does not increment on next byte fetch
            opcode = mem.get_byte(current_pc);
            halt_bug_flag = false;
        }
        else
        {
            opcode = mem.fetch_byte();
        }

        // --- Critical Value Logging ---
        // (Optional: limit logging to specific PC ranges or once test execution begins)
        static uint32_t log_count = 0;
        if (log_count < 100000)
        {
            DebugLogger::instance().log_step(
                current_pc,
                opcode,
                mem.reg_get16(Register16Bit::AF),
                mem.reg_get16(Register16Bit::BC),
                mem.reg_get16(Register16Bit::DE),
                mem.reg_get16(Register16Bit::HL),
                mem.get_sp(),
                timer.read_byte(0xFF04), // DIV
                timer.read_byte(0xFF05), // TIMA
                timer.read_byte(0xFF06), // TMA
                timer.read_byte(0xFF07), // TAC
                mem.get_byte(A_IFLAGS),
                mem.get_byte(A_IENABLE),
                ir.get_ime(),
                ticks
            );
        }

        process_instruction(opcode);

        // Process EI delay: takes effect after the instruction following EI completes
        ir.process_ei_count();
    }

    return ticks;
}

void Cpu::add_ticks(u8 extra_ticks)
{
    ticks += extra_ticks;
}

void Cpu::process_instruction(u8 rom_byte)
{
    u8 temp_8bit_value;
    i8 signed_8bit_immediate;
    u16 unsigned_16bit_immediate;

    switch(rom_byte)
    {
        case 0x3e:  // Load immediate value n to A register
            mem.reg_set(RA, mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x06:  // Load immediate value n to B register
            mem.reg_set(RB, mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x0e:  // Load immediate value n to C register
            mem.reg_set(RC, mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x16:  // Load immediate value n to D register
            mem.reg_set(RD, mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x1e:  // Load immediate value n to E register
            mem.reg_set(RE, mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x26:  // Load immediate value n to H register
            mem.reg_set(RH, mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x2e:  // Load immediate value n to L register
            mem.reg_set(RL, mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x7f:  // Set register A to register A
            mem.reg_copy(RA, RA);
            add_ticks(4);
            break;
        case 0x78:  // Set register A to register B
            mem.reg_copy(RA, RB);
            add_ticks(4);
            break;
        case 0x79:  // Set register A to register C
            mem.reg_copy(RA, RC);
            add_ticks(4);
            break;
        case 0x7a:  // Set register A to register D
            mem.reg_copy(RA, RD);
            add_ticks(4);
            break;
        case 0x7b:  // Set register A to register E
            mem.reg_copy(RA, RE);
            add_ticks(4);
            break;
        case 0x7c:  // Set register A to register H
            mem.reg_copy(RA, RH);
            add_ticks(4);
            break;
        case 0x7d:  // Set register A to register L
            mem.reg_copy(RA, RL);
            add_ticks(4);
            break;
        case 0x7e:  // Set register A to value at (HL)
            mem.reg_set(RA, mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0x47:  // Set register B to register A
            mem.reg_copy(RB, RA);
            add_ticks(4);
            break;
        case 0x40:  // Set register B to register B
            mem.reg_copy(RB, RB);
            add_ticks(4);
            break;
        case 0x41:  // Set register B to register C
            mem.reg_copy(RB, RC);
            add_ticks(4);
            break;
        case 0x42:  // Set register B to register D
            mem.reg_copy(RB, RD);
            add_ticks(4);
            break;
        case 0x43:  // Set register B to register E
            mem.reg_copy(RB, RE);
            add_ticks(4);
            break;
        case 0x44:  // Set register B to register H
            mem.reg_copy(RB, RH);
            add_ticks(4);
            break;
        case 0x45:  // Set register B to register L
            mem.reg_copy(RB, RL);
            add_ticks(4);
            break;
        case 0x46:  // Set register B to value at (HL)
            mem.reg_set(RB, mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0x4f:  // Set register C to register A
            mem.reg_copy(RC, RA);
            add_ticks(4);
            break;
        case 0x48:  // Set register C to register B
            mem.reg_copy(RC, RB);
            add_ticks(4);
            break;
        case 0x49:  // Set register C to register C
            mem.reg_copy(RC, RC);
            add_ticks(4);
            break;
        case 0x4a:  // Set register C to register D
            mem.reg_copy(RC, RD);
            add_ticks(4);
            break;
        case 0x4b:  // Set register C to register E
            mem.reg_copy(RC, RE);
            add_ticks(4);
            break;
        case 0x4c:  // Set register C to register H
            mem.reg_copy(RC, RH);
            add_ticks(4);
            break;
        case 0x4d:  // Set register C to register L
            mem.reg_copy(RC, RL);
            add_ticks(4);
            break;
        case 0x4e:  // Set register C to value at (HL)
            mem.reg_set(RC, mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0x57:  // Set register D to register A
            mem.reg_copy(RD, RA);
            add_ticks(4);
            break;
        case 0x50:  // Set register D to register B
            mem.reg_copy(RD, RB);
            add_ticks(4);
            break;
        case 0x51:  // Set register D to register C
            mem.reg_copy(RD, RC);
            add_ticks(4);
            break;
        case 0x52:  // Set register D to register D
            mem.reg_copy(RD, RD);
            add_ticks(4);
            break;
        case 0x53:  // Set register D to register E
            mem.reg_copy(RD, RE);
            add_ticks(4);
            break;
        case 0x54:  // Set register D to register H
            mem.reg_copy(RD, RH);
            add_ticks(4);
            break;
        case 0x55:  // Set register D to register L
            mem.reg_copy(RD, RL);
            add_ticks(4);
            break;
        case 0x56:  // Set register D to value at (HL)
            mem.reg_set(RD, mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0x5f:  // Set register E to register A
            mem.reg_copy(RE, RA);
            add_ticks(4);
            break;
        case 0x58:  // Set register E to register B
            mem.reg_copy(RE, RB);
            add_ticks(4);
            break;
        case 0x59:  // Set register E to register C
            mem.reg_copy(RE, RC);
            add_ticks(4);
            break;
        case 0x5a:  // Set register E to register D
            mem.reg_copy(RE, RD);
            add_ticks(4);
            break;
        case 0x5b:  // Set register E to register E
            mem.reg_copy(RE, RE);
            add_ticks(4);
            break;
        case 0x5c:  // Set register E to register H
            mem.reg_copy(RE, RH);
            add_ticks(4);
            break;
        case 0x5d:  // Set register E to register L
            mem.reg_copy(RE, RL);
            add_ticks(4);
            break;
        case 0x5e:  // Set register E to value at (HL)
            mem.reg_set(RE, mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0x67:  // Set register H to register A
            mem.reg_copy(RH, RA);
            add_ticks(4);
            break;
        case 0x60:  // Set register H to register B
            mem.reg_copy(RH, RB);
            add_ticks(4);
            break;
        case 0x61:  // Set register H to register C
            mem.reg_copy(RH, RC);
            add_ticks(4);
            break;
        case 0x62:  // Set register H to register D
            mem.reg_copy(RH, RD);
            add_ticks(4);
            break;
        case 0x63:  // Set register H to register E
            mem.reg_copy(RH, RE);
            add_ticks(4);
            break;
        case 0x64:  // Set register H to register H
            mem.reg_copy(RH, RH);
            add_ticks(4);
            break;
        case 0x65:  // Set register H to register L
            mem.reg_copy(RH, RL);
            add_ticks(4);
            break;
        case 0x66:  // Set register H to value at (HL)
            mem.reg_set(RH, mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0x6f:  // Set register L to register A
            mem.reg_copy(RL, RA);
            add_ticks(4);
            break;
        case 0x68:  // Set register L to register B
            mem.reg_copy(RL, RB);
            add_ticks(4);
            break;
        case 0x69:  // Set register L to register C
            mem.reg_copy(RL, RC);
            add_ticks(4);
            break;
        case 0x6a:  // Set register L to register D
            mem.reg_copy(RL, RD);
            add_ticks(4);
            break;
        case 0x6b:  // Set register L to register E
            mem.reg_copy(RL, RE);
            add_ticks(4);
            break;
        case 0x6c:  // Set register L to register H
            mem.reg_copy(RL, RH);
            add_ticks(4);
            break;
        case 0x6d:  // Set register L to register L
            mem.reg_copy(RL, RL);
            add_ticks(4);
            break;
        case 0x6e:  // Set register L to value at (HL)
            mem.reg_set(RL, mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0x70:  // Set byte at (HL) to register B
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RB));
            add_ticks(8);
            break;
        case 0x71:  // Set byte at (HL) to register C
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RC));
            add_ticks(8);
            break;
        case 0x72:  // Set byte at (HL) to register D
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RD));
            add_ticks(8);
            break;
        case 0x73:  // Set byte at (HL) to register E
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RE));
            add_ticks(8);
            break;
        case 0x74:  // Set byte at (HL) to register H
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RH));
            add_ticks(8);
            break;
        case 0x75:  // Set byte at (HL) to register L
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RL));
            add_ticks(8);
            break;
        case 0x36:  // Set byte at (HL) to immediate byte n
            mem.set_from_pointer(Register16Bit::HL,mem.fetch_byte());
            add_ticks(12);
            break;
        case 0x0a:  // Set register A to value at (BC)
            mem.reg_set(RA, mem.get_from_pointer(Register16Bit::BC));
            add_ticks(8);
            break;
        case 0x1a:  // Set register A to value at (DE)
            mem.reg_set(RA, mem.get_from_pointer(Register16Bit::DE));
            add_ticks(8);
            break;
        case 0xfa:  // Set register A to immediate value at (nn)
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            mem.reg_set(RA,mem.get_byte(unsigned_16bit_immediate));
            add_ticks(16);
            break;
        case 0x02:  // Set byte at (BC) to register A
            mem.set_from_pointer(Register16Bit::BC,mem.reg_get(RA));
            add_ticks(8);
            break;
        case 0x12:  // Set byte at (DE) to register A
            mem.set_from_pointer(Register16Bit::DE,mem.reg_get(RA));
            add_ticks(8);
            break;
        case 0x77:  // Set byte at (HL) to register A
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RA));
            add_ticks(8);
            break;
        case 0xea:  // Set byte at (nn) to register A
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            mem.write_byte(unsigned_16bit_immediate,mem.reg_get(RA));
            add_ticks(16);
            break;
        case 0xf2:  // Set register A to ($FF00 + register C)
            unsigned_16bit_immediate = 0xff00 + mem.reg_get(RC);
            mem.reg_set(RA,mem.get_byte(unsigned_16bit_immediate));
            add_ticks(8);
            break;
        case 0xe2:  // Set byte at ($FF00 + register C) to register A
            unsigned_16bit_immediate = 0xff00 + mem.reg_get(RC);
            mem.write_byte(unsigned_16bit_immediate,mem.reg_get(RA));
            add_ticks(8);
            break;
        case 0x3a:  // Set register A to value at (HL), decrement HL
            mem.reg_set(RA, mem.get_from_pointer(Register16Bit::HL));
            unsigned_16bit_immediate = mem.reg_get16(Register16Bit::HL) - 1;
            mem.reg_set(Register16Bit::HL, unsigned_16bit_immediate);
            add_ticks(8);
            break;
        case 0x32:  // Set byte at (HL) to register A, decrement HL
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RA));
            unsigned_16bit_immediate = mem.reg_get16(Register16Bit::HL) - 1;
            mem.reg_set(Register16Bit::HL, unsigned_16bit_immediate);
            add_ticks(8);
            break;
        case 0x2a:  // Set register A to value at (HL), increment HL
            mem.reg_set(RA, mem.get_from_pointer(Register16Bit::HL));
            unsigned_16bit_immediate = mem.reg_get16(Register16Bit::HL) + 1;
            mem.reg_set(Register16Bit::HL, unsigned_16bit_immediate);
            add_ticks(8);
            break;
        case 0x22:  // Set byte at (HL) to register A, increment HL
            mem.set_from_pointer(Register16Bit::HL,mem.reg_get(RA));
            unsigned_16bit_immediate = mem.reg_get16(Register16Bit::HL) + 1;
            mem.reg_set(Register16Bit::HL, unsigned_16bit_immediate);
            add_ticks(8);
            break;
        case 0xe0:  // Set byte at ($FF00 + imm8 n) to register A
            unsigned_16bit_immediate = 0xff00 + mem.fetch_byte();
            mem.write_byte(unsigned_16bit_immediate,mem.reg_get(RA));
            add_ticks(12);
            break;
        case 0xf0:  // Set register A to ($FF00 + imm8 n)
            unsigned_16bit_immediate = 0xff00 + mem.fetch_byte();
            mem.reg_set(RA,mem.get_byte(unsigned_16bit_immediate));
            add_ticks(12);
            break;
        case 0x01:  // Set register BC to 16 bit immediate value nn
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            mem.reg_set(Register16Bit::BC,unsigned_16bit_immediate);
            add_ticks(12);
            break;
        case 0x11:  // Set register DE to 16 bit immediate value nn
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            mem.reg_set(Register16Bit::DE,unsigned_16bit_immediate);
            add_ticks(12);
            break;
        case 0x21:  // Set register HL to 16 bit immediate value nn
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            mem.reg_set(Register16Bit::HL,unsigned_16bit_immediate);
            add_ticks(12);
            break;
        case 0x31:  // Set SP to 16 bit immediate value nn
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            mem.set_sp(unsigned_16bit_immediate);
            add_ticks(12);
            break;
        case 0xf9:  // Set SP to value in HL
            mem.set_sp(mem.reg_get16(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0xf8:  // Set HL to SP + imm8 n
            {
                int8_t signed_offset = static_cast<int8_t>(mem.fetch_byte());
                u16 sp_val = mem.get_sp();
                u32 result = sp_val + signed_offset;

                // Flags: H and C are evaluated on the lower byte addition
                mem.flag_update(HF, ((sp_val & 0x0F) + (static_cast<u8>(signed_offset) & 0x0F)) > 0x0F);
                mem.flag_update(CF, ((sp_val & 0xFF) + (static_cast<u8>(signed_offset) & 0xFF)) > 0xFF);
                mem.flag_update(ZF, 0);
                mem.flag_update(NF, 0);

                mem.reg_set(Register16Bit::HL, static_cast<u16>(result));
                add_ticks(12);
                break;
            }
        case 0x08:  // Set bytes at (nn) to SP value
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            // sp low byte
            temp_8bit_value = mem.get_sp() & 0xff;
            mem.write_byte(unsigned_16bit_immediate,temp_8bit_value);
            // sp high byte
            temp_8bit_value = mem.get_sp() >> 8;
            mem.write_byte(unsigned_16bit_immediate + 1,temp_8bit_value);
            add_ticks(20);
            break;
        case 0xf5:  // Push AF register onto stack
            mem.stack_push(mem.reg_get16(Register16Bit::AF));
            add_ticks(16);
            break;
        case 0xc5:  // Push BC register onto stack
            mem.stack_push(mem.reg_get16(Register16Bit::BC));
            add_ticks(16);
            break;
        case 0xd5:  // Push DE register onto stack
            mem.stack_push(mem.reg_get16(Register16Bit::DE));
            add_ticks(16);
            break;
        case 0xe5:  // Push HL register onto stack
            mem.stack_push(mem.reg_get16(Register16Bit::HL));
            add_ticks(16);
            break;
        case 0xf1:  // Pop stack 16-bit value to AF register
            mem.reg_set(Register16Bit::AF,mem.stack_pop());
            add_ticks(12);
            break;
        case 0xc1:  // Pop stack 16-bit value to BC register
            mem.reg_set(Register16Bit::BC,mem.stack_pop());
            add_ticks(12);
            break;
        case 0xd1:  // Pop stack 16-bit value to DE register
            mem.reg_set(Register16Bit::DE,mem.stack_pop());
            add_ticks(12);
            break;
        case 0xe1:  // Pop stack 16-bit value to HL register
            mem.reg_set(Register16Bit::HL,mem.stack_pop());
            add_ticks(12);
            break;
        case 0x87:  // Set register A to reg A + reg A
            mem.reg_add(RA, mem.reg_get(RA));
            add_ticks(4);
            break;
        case 0x80:  // Set register A to reg A + reg B
            mem.reg_add(RA, mem.reg_get(RB));
            add_ticks(4);
            break;
        case 0x81:  // Set register A to reg A + reg C
            mem.reg_add(RA, mem.reg_get(RC));
            add_ticks(4);
            break;
        case 0x82:  // Set register A to reg A + reg D
            mem.reg_add(RA, mem.reg_get(RD));
            add_ticks(4);
            break;
        case 0x83:  // Set register A to reg A + reg E
            mem.reg_add(RA, mem.reg_get(RE));
            add_ticks(4);
            break;
        case 0x84:  // Set register A to reg A + reg H
            mem.reg_add(RA, mem.reg_get(RH));
            add_ticks(4);
            break;
        case 0x85:  // Set register A to reg A + reg L
            mem.reg_add(RA, mem.reg_get(RL));
            add_ticks(4);
            break;
        case 0x86:  // Set register A to reg A + value at (HL)
            mem.reg_add(RA, mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0xc6:  // Set register A to reg A + imm8 n
            mem.reg_add(RA, mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x8f:  // Set register A to reg A + reg A + carry
            mem.reg_add(RA,mem.reg_get(RA),true);
            add_ticks(4);
            break;
        case 0x88:  // Set register A to reg A + reg B + carry
            mem.reg_add(RA,mem.reg_get(RB),true);
            add_ticks(4);
            break;
        case 0x89:  // Set register A to reg A + reg C + carry
            mem.reg_add(RA,mem.reg_get(RC),true);
            add_ticks(4);
            break;
        case 0x8a:  // Set register A to reg A + reg D + carry
            mem.reg_add(RA,mem.reg_get(RD),true);
            add_ticks(4);
            break;
        case 0x8b:  // Set register A to reg A + reg E + carry
            mem.reg_add(RA,mem.reg_get(RE),true);
            add_ticks(4);
            break;
        case 0x8c:  // Set register A to reg A + reg H + carry
            mem.reg_add(RA,mem.reg_get(RH),true);
            add_ticks(4);
            break;
        case 0x8d:  // Set register A to reg A + reg L + carry
            mem.reg_add(RA,mem.reg_get(RL),true);
            add_ticks(4);
            break;
        case 0x8e:  // Set register A to reg A + (HL) + carry
            mem.reg_add(RA,mem.get_from_pointer(Register16Bit::HL),true);
            add_ticks(8);
            break;
        case 0xce:  // Set register A to reg A + imm8 n + carry
            mem.reg_add(RA,mem.fetch_byte(),true);
            add_ticks(8);
            break;
        case 0x97:  // Set register A to reg A minus reg A
            mem.reg_sub(RA,mem.reg_get(RA));
            add_ticks(4);
            break;
        case 0x90:  // Set register A to reg A minus reg B
            mem.reg_sub(RA,mem.reg_get(RB));
            add_ticks(4);
            break;
        case 0x91:  // Set register A to reg A minus reg C
            mem.reg_sub(RA,mem.reg_get(RC));
            add_ticks(4);
            break;
        case 0x92:  // Set register A to reg A minus reg D
            mem.reg_sub(RA,mem.reg_get(RD));
            add_ticks(4);
            break;
        case 0x93:  // Set register A to reg A minus reg E
            mem.reg_sub(RA,mem.reg_get(RE));
            add_ticks(4);
            break;
        case 0x94:  // Set register A to reg A minus reg H
            mem.reg_sub(RA,mem.reg_get(RH));
            add_ticks(4);
            break;
        case 0x95:  // Set register A to reg A minus reg L
            mem.reg_sub(RA,mem.reg_get(RL));
            add_ticks(4);
            break;
        case 0x96:  // Set register A to reg A minus (HL)
            mem.reg_sub(RA,mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0xd6:  // Set register A to reg A minus imm8 n
            mem.reg_sub(RA,mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x9f:  // Set register A to reg A minus reg A - carry
            mem.reg_sub(RA,mem.reg_get(RA),true);
            add_ticks(4);
            break;
        case 0x98:  // Set register A to reg A minus reg B - carry
            mem.reg_sub(RA,mem.reg_get(RB),true);
            add_ticks(4);
            break;
        case 0x99:  // Set register A to reg A minus reg C - carry
            mem.reg_sub(RA,mem.reg_get(RC),true);
            add_ticks(4);
            break;
        case 0x9a:  // Set register A to reg A minus reg D - carry
            mem.reg_sub(RA,mem.reg_get(RD),true);
            add_ticks(4);
            break;
        case 0x9b:  // Set register A to reg A minus reg E - carry
            mem.reg_sub(RA,mem.reg_get(RE),true);
            add_ticks(4);
            break;
        case 0x9c:  // Set register A to reg A minus reg H - carry
            mem.reg_sub(RA,mem.reg_get(RH),true);
            add_ticks(4);
            break;
        case 0x9d:  // Set register A to reg A minus reg L - carry
            mem.reg_sub(RA,mem.reg_get(RL),true);
            add_ticks(4);
            break;
        case 0x9e:  // Set register A to reg A minus (HL) - carry
            mem.reg_sub(RA,mem.get_from_pointer(Register16Bit::HL),true);
            add_ticks(8);
            break;
        case 0xde:  // Set register A to reg A minus imm8 n - carry
            mem.reg_sub(RA,mem.fetch_byte(),true);
            add_ticks(8);
            break;
        case 0xa7:  // Set register A to reg A AND reg A
            mem.reg_and(mem.reg_get(RA));
            add_ticks(4);
            break;
        case 0xa0:  // Set register A to reg A AND reg B
            mem.reg_and(mem.reg_get(RB));
            add_ticks(4);
            break;
        case 0xa1:  // Set register A to reg A AND reg C
            mem.reg_and(mem.reg_get(RC));
            add_ticks(4);
            break;
        case 0xa2:  // Set register A to reg A AND reg D
            mem.reg_and(mem.reg_get(RD));
            add_ticks(4);
            break;
        case 0xa3:  // Set register A to reg A AND reg E
            mem.reg_and(mem.reg_get(RE));
            add_ticks(4);
            break;
        case 0xa4:  // Set register A to reg A AND reg H
            mem.reg_and(mem.reg_get(RH));
            add_ticks(4);
            break;
        case 0xa5:  // Set register A to reg A AND reg L
            mem.reg_and(mem.reg_get(RL));
            add_ticks(4);
            break;
        case 0xa6:  // Set register A to reg A AND (HL)
            mem.reg_and(mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0xe6:  // Set register A to reg A AND imm8 n
            mem.reg_and(mem.fetch_byte());
            add_ticks(8);
            break;
        case 0xb7:  // Set register A to reg A OR reg A
            mem.reg_or(mem.reg_get(RA));
            add_ticks(4);
            break;
        case 0xb0:  // Set register A to reg A OR reg B
            mem.reg_or(mem.reg_get(RB));
            add_ticks(4);
            break;
        case 0xb1:  // Set register A to reg A OR reg C
            mem.reg_or(mem.reg_get(RC));
            add_ticks(4);
            break;
        case 0xb2:  // Set register A to reg A OR reg D
            mem.reg_or(mem.reg_get(RD));
            add_ticks(4);
            break;
        case 0xb3:  // Set register A to reg A OR reg E
            mem.reg_or(mem.reg_get(RE));
            add_ticks(4);
            break;
        case 0xb4:  // Set register A to reg A OR reg H
            mem.reg_or(mem.reg_get(RH));
            add_ticks(4);
            break;
        case 0xb5:  // Set register A to reg A OR reg L
            mem.reg_or(mem.reg_get(RL));
            add_ticks(4);
            break;
        case 0xb6:  // Set register A to reg A OR (HL)
            mem.reg_or(mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0xf6:  // Set register A to reg A OR imm8 n
            mem.reg_or(mem.fetch_byte());
            add_ticks(8);
            break;
        case 0xaf:  // Set register A to reg A XOR reg A
            mem.reg_xor(mem.reg_get(RA));
            add_ticks(4);
            break;
        case 0xa8:  // Set register A to reg A XOR reg B
            mem.reg_xor(mem.reg_get(RB));
            add_ticks(4);
            break;
        case 0xa9:  // Set register A to reg A XOR reg C
            mem.reg_xor(mem.reg_get(RC));
            add_ticks(4);
            break;
        case 0xaa:  // Set register A to reg A XOR reg D
            mem.reg_xor(mem.reg_get(RD));
            add_ticks(4);
            break;
        case 0xab:  // Set register A to reg A XOR reg E
            mem.reg_xor(mem.reg_get(RE));
            add_ticks(4);
            break;
        case 0xac:  // Set register A to reg A XOR reg H
            mem.reg_xor(mem.reg_get(RH));
            add_ticks(4);
            break;
        case 0xad:  // Set register A to reg A XOR reg L
            mem.reg_xor(mem.reg_get(RL));
            add_ticks(4);
            break;
        case 0xae:  // Set register A to reg A XOR (HL)
            mem.reg_xor(mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0xee:  // Set register A to reg A XOR imm8 n
            mem.reg_xor(mem.fetch_byte());
            add_ticks(8);
            break;
        case 0xbf:  // Compare register A to register A and update flags
            mem.reg_compare(mem.reg_get(RA));
            add_ticks(4);
            break;
        case 0xb8:  // Compare register A to register B and update flags
            mem.reg_compare(mem.reg_get(RB));
            add_ticks(4);
            break;
        case 0xb9:  // Compare register A to register C and update flags
            mem.reg_compare(mem.reg_get(RC));
            add_ticks(4);
            break;
        case 0xba:  // Compare register A to register D and update flags
            mem.reg_compare(mem.reg_get(RD));
            add_ticks(4);
            break;
        case 0xbb:  // Compare register A to register E and update flags
            mem.reg_compare(mem.reg_get(RE));
            add_ticks(4);
            break;
        case 0xbc:  // Compare register A to register H and update flags
            mem.reg_compare(mem.reg_get(RH));
            add_ticks(4);
            break;
        case 0xbd:  // Compare register A to register L and update flags
            mem.reg_compare(mem.reg_get(RL));
            add_ticks(4);
            break;
        case 0xbe:  // Compare register A to (HL) and update flags
            mem.reg_compare(mem.get_from_pointer(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0xfe:  // Compare register A to imm8 n and update flags
            mem.reg_compare(mem.fetch_byte());
            add_ticks(8);
            break;
        case 0x3c:  // Increment register A by 1 and update flags
            mem.reg_inc(RA);
            add_ticks(4);
            break;
        case 0x04:  // Increment register B by 1 and update flags
            mem.reg_inc(RB);
            add_ticks(4);
            break;
        case 0x0c:  // Increment register C by 1 and update flags
            mem.reg_inc(RC);
            add_ticks(4);
            break;
        case 0x14:  // Increment register D by 1 and update flags
            mem.reg_inc(RD);
            add_ticks(4);
            break;
        case 0x1c:  // Increment register E by 1 and update flags
            mem.reg_inc(RE);
            add_ticks(4);
            break;
        case 0x24:  // Increment register H by 1 and update flags
            mem.reg_inc(RH);
            add_ticks(4);
            break;
        case 0x2c:  // Increment register L by 1 and update flags
            mem.reg_inc(RL);
            add_ticks(4);
            break;
        case 0x34:  // Increment byte at (HL) by 1 and update flags
            mem.inc_from_pointer(Register16Bit::HL);
            add_ticks(12);
            break;
        case 0x3d:  // Decrement register A by 1 and update flags
            mem.reg_dec(RA);
            add_ticks(4);
            break;
        case 0x05:  // Decrement register B by 1 and update flags
            mem.reg_dec(RB);
            add_ticks(4);
            break;
        case 0x0d:  // Decrement register C by 1 and update flags
            mem.reg_dec(RC);
            add_ticks(4);
            break;
        case 0x15:  // Decrement register D by 1 and update flags
            mem.reg_dec(RD);
            add_ticks(4);
            break;
        case 0x1d:  // Decrement register E by 1 and update flags
            mem.reg_dec(RE);
            add_ticks(4);
            break;
        case 0x25:  // Decrement register H by 1 and update flags
            mem.reg_dec(RH);
            add_ticks(4);
            break;
        case 0x2d:  // Decrement register L by 1 and update flags
            mem.reg_dec(RL);
            add_ticks(4);
            break;
        case 0x35:  // Decrement byte at (HL) by 1 and update flags
            mem.dec_from_pointer(Register16Bit::HL);
            add_ticks(12);
            break;
        case 0x09:  // Register HL is set to register HL + register BC, flags updated
            mem.reg_add16(Register16Bit::HL,mem.reg_get16(Register16Bit::BC));
            add_ticks(8);
            break;
        case 0x19:  // Register HL is set to register HL + register DE, flags updated
            mem.reg_add16(Register16Bit::HL,mem.reg_get16(Register16Bit::DE));
            add_ticks(8);
            break;
        case 0x29:  // Register HL is set to register HL + register HL, flags updated
            mem.reg_add16(Register16Bit::HL,mem.reg_get16(Register16Bit::HL));
            add_ticks(8);
            break;
        case 0x39:  // Register HL is set to register HL + SP, flags updated
            mem.reg_add16(Register16Bit::HL,mem.get_sp());
            add_ticks(8);
            break;
        case 0xe8:  // SP is set to SP + imm8 n, flags updated
            mem.sp_add(mem.fetch_byte());
            add_ticks(16);
            break;
        case 0x03:  // Increment BC register by 1, flags not updated
            mem.reg_inc16(Register16Bit::BC);
            add_ticks(8);
            break;
        case 0x13:  // Increment DE register by 1, flags not updated
            mem.reg_inc16(Register16Bit::DE);
            add_ticks(8);
            break;
        case 0x23:  // Increment HL register by 1, flags not updated
            mem.reg_inc16(Register16Bit::HL);
            add_ticks(8);
            break;
        case 0x33:  // Increment SP register by 1, flags not updated
            mem.inc_sp(1);
            add_ticks(8);
            break;
        case 0x0b:  // Decrement BC register by 1, flags not updated
            mem.reg_dec16(Register16Bit::BC);
            add_ticks(8);
            break;
        case 0x1b:  // Decrement DE register by 1, flags not updated
            mem.reg_dec16(Register16Bit::DE);
            add_ticks(8);
            break;
        case 0x2b:  // Decrement HL register by 1, flags not updated
            mem.reg_dec16(Register16Bit::HL);
            add_ticks(8);
            break;
        case 0x3b:  // Decrement SP register by 1, flags not updated
            mem.dec_sp(1);
            add_ticks(8);
            break;
        case 0xcb:  // Process a CB prefixed instruction
            process_cb_instruction(mem.fetch_byte()); // Cycles are added as instructions are processed
            break;
        case 0x27:  // Decimal Adjust register A (DAA)
            mem.reg_daa();
            add_ticks(4);
            break;
        case 0x2f:  // Complement A register and update flags
            temp_8bit_value = ~mem.reg_get(RA);
            mem.reg_set(RA, temp_8bit_value);
            mem.flag_update(NF,1);
            mem.flag_update(HF,1);
            add_ticks(4);
            break;
        case 0x3f:  // Complement carry flag
            mem.flag_update(CF,!mem.flag_get(CF));
            mem.flag_update(NF,0);
            mem.flag_update(HF,0);
            add_ticks(4);
            break;
        case 0x37:  // Set carry flag
            mem.flag_update(CF,1);
            mem.flag_update(NF,0);
            mem.flag_update(HF,0);
            add_ticks(4);
            break;
        case 0x00:  // No operation (NOP)
            add_ticks(4);
            break;
        case 0x76: // HALT
            {
                u8 i_flags = mem.get_byte(A_IFLAGS);
                u8 i_enable = mem.get_byte(A_IENABLE);
                bool interrupt_pending = (i_flags & i_enable & 0x1F) != 0;

                if (!ir.get_ime())
                {
                    if (interrupt_pending)
                    {
                        // HALT Bug: IME is 0, but an interrupt is already requested.
                        // The CPU does NOT sleep, but PC fails to advance on the next byte fetch.
                        halt_bug_flag = true;
                    }
                    else
                    {
                        // IME is 0 and no interrupt is pending: CPU sleeps until an interrupt flag is set
                        halt();
                    }
                }
                else
                {
                    // IME is 1: normal halt behavior
                    halt();
                }

                add_ticks(4);
                break;
            }
        case 0x10:  // Stop the CPU and LCD until a button is pressed (STOP)
            mem.fetch_byte(); // Byte should be 0x00
            stop();
            add_ticks(4);
            break;
        case 0xf3:  // Disable interrupts (DI)
            ir.disable_interrupts();
            add_ticks(4);
            break;
        case 0xfb:  // Enable interrupts (EI) - happens after next instruction
            ir.ei_count = 2; // Set ei counter so that ei is effective after next instruction
            add_ticks(4);
            break;
        case 0x07:  // Rotate contents of A register left and store bit 7 in CF, flags updated
            mem.reg_rla(true); // Carry = true (Rotate left with carry RLCA)
            add_ticks(4);
            break;
        case 0x17:  // Rotate contents of A register left and store bit 7 in CF, flags updated
            mem.reg_rla(false); // Carry = false (Rotate left RLA)
            add_ticks(4);
            break;
        case 0x0f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            mem.reg_rra(true); // Carry = true (Rotate right with carry RRCA)
            add_ticks(4);
            break;
        case 0x1f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            mem.reg_rra(false); // Carry = false (Rotate right RRA)
            add_ticks(4);
            break;
        case 0xc3:  // Jump to subroutine at nn
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xc2:  // Jump to subroutine at nn if ZF is reset
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            if (!mem.flag_get(ZF))
            {
                mem.set_pc(unsigned_16bit_immediate);
                add_ticks(16);
            }
            else
            {
                //mem.inc_pc(1);
                add_ticks(12);
            }
            break;
        case 0xca:  // Jump to subroutine at nn if ZF is set
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            if (mem.flag_get(ZF))
            {
                mem.set_pc(unsigned_16bit_immediate);
                add_ticks(16);
            }
            else
            {
                //mem.inc_pc(1);
                add_ticks(12);
            }
            break;
        case 0xd2:  // Jump to subroutine at nn if CF is reset
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            if (!mem.flag_get(CF))
            {
                mem.set_pc(unsigned_16bit_immediate);
                add_ticks(16);
            }
            else
            {
                //mem.inc_pc(1);
                add_ticks(12);
            }
            break;
        case 0xda:  // Jump to subroutine at nn if CF is set
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            if (mem.flag_get(CF))
            {
                mem.set_pc(unsigned_16bit_immediate);
                add_ticks(16);
            }
            else
            {
                //mem.inc_pc(1);
                add_ticks(12);
            }
            break;
        case 0xe9:  // Jump to address contained in HL
            mem.set_pc(mem.reg_get16(Register16Bit::HL));
            add_ticks(4);
            break;
        case 0x18:  // Jump to address at PC + e (e = signed 8-bit immediate)
            signed_8bit_immediate = mem.fetch_byte();
            mem.jmp_n(signed_8bit_immediate);
            add_ticks(12);
            break;
        case 0x20:  // Jump to address at PC + e if ZF is reset (e = signed 8-bit immediate)
            signed_8bit_immediate = mem.fetch_byte();
            if (!mem.flag_get(ZF))
            {
                mem.jmp_n(signed_8bit_immediate);
                add_ticks(12);
            }
            else
            {
                //mem.inc_pc(1);
                add_ticks(8);
            }
            break;
        case 0x28:  // Jump to address at PC + e if ZF is set (e = signed 8-bit immediate)
            signed_8bit_immediate = mem.fetch_byte();
            if (mem.flag_get(ZF))
            {
                mem.jmp_n(signed_8bit_immediate);
                add_ticks(12);
            }
            else
            {
                //mem.inc_pc(1);
                add_ticks(8);
            }
            break;
        case 0x30:  // Jump to address at PC + e if CF is reset (e = signed 8-bit immediate)
            signed_8bit_immediate = mem.fetch_byte();
            if (!mem.flag_get(CF))
            {
                mem.jmp_n(signed_8bit_immediate);
                add_ticks(12);
            }
            else
            {
                //mem.inc_pc(1);
                add_ticks(8);
            }
            break;
        case 0x38:  // Jump to address at PC + e if CF is set (e = signed 8-bit immediate)
            signed_8bit_immediate = mem.fetch_byte();
            if (mem.flag_get(CF))
            {
                mem.jmp_n(signed_8bit_immediate);
                add_ticks(12);
            }
            else
            {
                //mem.inc_pc(1);
                add_ticks(8);
            }
            break;
        case 0xcd:  // Push PC onto stack and jmp to nn (CALL)
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(24);
            break;
        case 0xc4:  // Push PC onto stack and jmp to nn (CALL) if ZF is reset
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;

            if (!mem.flag_get(ZF))
            {
                mem.pc_push();
                mem.set_pc(unsigned_16bit_immediate);
                add_ticks(24);
            }
            else
            {
                add_ticks(12);
            }
            break;
        case 0xcc:  // Push PC onto stack and jmp to nn (CALL) if ZF is set
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;

            if (mem.flag_get(ZF))
            {
                mem.pc_push();
                mem.set_pc(unsigned_16bit_immediate);
                add_ticks(24);
            }
            else
            {
                add_ticks(12);
            }
            break;
        case 0xd4:  // Push PC onto stack and jmp to nn (CALL) if CF is reset
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;

            if (!mem.flag_get(CF))
            {
                mem.pc_push();
                mem.set_pc(unsigned_16bit_immediate);
                add_ticks(24);
            }
            else
            {
                add_ticks(12);
            }
            break;
        case 0xdc:  // Push PC onto stack and jmp to nn (CALL) if CF is set
            unsigned_16bit_immediate = mem.fetch_byte();
            unsigned_16bit_immediate += mem.fetch_byte() << 8;

            if (mem.flag_get(CF))
            {
                mem.pc_push();
                mem.set_pc(unsigned_16bit_immediate);
                add_ticks(24);
            }
            else
            {
                add_ticks(12);
            }
            break;
        case 0xc7:  // Push PC onto stack and jmp to address 0x0000 + 0x00 (RST)
            unsigned_16bit_immediate = 0x0000;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xcf:  // Push PC onto stack and jmp to address 0x0000 + 0x08 (RST)
            unsigned_16bit_immediate = 0x0008;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xd7:  // Push PC onto stack and jmp to address 0x0000 + 0x10 (RST)
            unsigned_16bit_immediate = 0x0010;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xdf:  // Push PC onto stack and jmp to address 0x0000 + 0x18 (RST)
            unsigned_16bit_immediate = 0x0018;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xe7:  // Push PC onto stack and jmp to address 0x0000 + 0x20 (RST)
            unsigned_16bit_immediate = 0x0020;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xef:  // Push PC onto stack and jmp to address 0x0000 + 0x28 (RST)
            unsigned_16bit_immediate = 0x0028;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xf7:  // Push PC onto stack and jmp to address 0x0000 + 0x30 (RST)
            unsigned_16bit_immediate = 0x0030;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xff:  // Push PC onto stack and jmp to address 0x0000 + 0x38 (RST)
            unsigned_16bit_immediate = 0x0038;
            mem.pc_push();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xc9:  // Pop 2 bytes from the stack and jump to that address (RET)
            unsigned_16bit_immediate = mem.stack_pop();
            mem.set_pc(unsigned_16bit_immediate);
            add_ticks(16);
            break;
        case 0xc0:  // Pop 2 bytes from the stack and return if ZF is reset (RET nz)
            if (!mem.flag_get(ZF)) {
                mem.set_pc(mem.stack_pop());
                add_ticks(20);
            } else {
                add_ticks(8);
            }
            break;
        case 0xc8:  // Pop 2 bytes from the stack and return if ZF is set (RET z)
            if (mem.flag_get(ZF)) {
                mem.set_pc(mem.stack_pop());
                add_ticks(20);
            } else {
                add_ticks(8);
            }
            break;
        case 0xd0:  // Pop 2 bytes from the stack and return if CF is reset (RET nc)
            if (!mem.flag_get(CF)) {
                mem.set_pc(mem.stack_pop());
                add_ticks(20);
            } else {
                add_ticks(8);
            }
            break;
        case 0xd8:  // Pop 2 bytes from the stack and return if CF is set (RET c)
            if (mem.flag_get(CF)) {
                mem.set_pc(mem.stack_pop());
                add_ticks(20);
            } else {
                add_ticks(8);
            }
            break;
        case 0xd9:  // Pop 2 bytes from the stack, jump to that address and enable interrupts (RETI)
            unsigned_16bit_immediate = mem.stack_pop();
            mem.set_pc(unsigned_16bit_immediate);
            ir.set_ime(true);   // Must set ime = true directly, NOT enable_interrupts()!
            ir.ei_count = 0;
            add_ticks(16);
            break;
    }
}

void Cpu::process_cb_instruction(u8 rom_byte)
{
    switch(rom_byte)
    {
        case 0x37:  // Swap upper and lower nibbles of register A, flags updated
            mem.reg_swap(RA);
            add_ticks(8);
            break;
        case 0x30:  // Swap upper and lower nibbles of register B, flags updated
            mem.reg_swap(RB);
            add_ticks(8);
            break;
        case 0x31:  // Swap upper and lower nibbles of register C, flags updated
            mem.reg_swap(RC);
            add_ticks(8);
            break;
        case 0x32:  // Swap upper and lower nibbles of register D, flags updated
            mem.reg_swap(RD);
            add_ticks(8);
            break;
        case 0x33:  // Swap upper and lower nibbles of register E, flags updated
            mem.reg_swap(RE);
            add_ticks(8);
            break;
        case 0x34:  // Swap upper and lower nibbles of register H, flags updated
            mem.reg_swap(RH);
            add_ticks(8);
            break;
        case 0x35:  // Swap upper and lower nibbles of register L, flags updated
            mem.reg_swap(RL);
            add_ticks(8);
            break;
        case 0x36:  // Swap upper and lower nibbles of byte at (HL), flags updated
            mem.swap_from_pointer(Register16Bit::HL);
            add_ticks(16);
            break;
        case 0x07:  // Rotate contents of A register left and store bit 7 in CF, flags updated
            mem.reg_rl(RA, true); // Carry = true (Rotate left with carry RLC A)
            add_ticks(8);
            break;
        case 0x00:  // Rotate contents of B register left and store bit 7 in CF, flags updated
            mem.reg_rl(RB, true); // Carry = true (Rotate left with carry RLC B)
            add_ticks(8);
            break;
        case 0x01:  // Rotate contents of C register left and store bit 7 in CF, flags updated
            mem.reg_rl(RC, true); // Carry = true (Rotate left with carry RLC C)
            add_ticks(8);
            break;
        case 0x02:  // Rotate contents of D register left and store bit 7 in CF, flags updated
            mem.reg_rl(RD, true); // Carry = true (Rotate left with carry RLC D)
            add_ticks(8);
            break;
        case 0x03:  // Rotate contents of E register left and store bit 7 in CF, flags updated
            mem.reg_rl(RE, true); // Carry = true (Rotate left with carry RLC E)
            add_ticks(8);
            break;
        case 0x04:  // Rotate contents of H register left and store bit 7 in CF, flags updated
            mem.reg_rl(RH, true); // Carry = true (Rotate left with carry RLC H)
            add_ticks(8);
            break;
        case 0x05:  // Rotate contents of L register left and store bit 7 in CF, flags updated
            mem.reg_rl(RL, true); // Carry = true (Rotate left with carry RLC L)
            add_ticks(8);
            break;
        case 0x06:  // Rotate contents of byte at (HL) left and store bit 7 in CF, flags updated
            mem.rl_from_pointer(Register16Bit::HL, true); // Carry = true (Rotate left with carry RLC L)
            add_ticks(16);
            break;
        case 0x17:  // Rotate contents of A register left through carry flag and store bit 7 in CF, flags updated
            mem.reg_rl(RA, false); // Carry = false (Rotate left RL A)
            add_ticks(8);
            break;
        case 0x10:  // Rotate contents of B register left through carry flag and store bit 7 in CF, flags updated
            mem.reg_rl(RB, false); // Carry = false (Rotate left with carry RL B)
            add_ticks(8);
            break;
        case 0x11:  // Rotate contents of C register left through carry flag and store bit 7 in CF, flags updated
            mem.reg_rl(RC, false); // Carry = false (Rotate left with carry RL C)
            add_ticks(8);
            break;
        case 0x12:  // Rotate contents of D register left through carry flag and store bit 7 in CF, flags updated
            mem.reg_rl(RD, false); // Carry = false (Rotate left with carry RL D)
            add_ticks(8);
            break;
        case 0x13:  // Rotate contents of E register left through carry flag and store bit 7 in CF, flags updated
            mem.reg_rl(RE, false); // Carry = false (Rotate left with carry RL E)
            add_ticks(8);
            break;
        case 0x14:  // Rotate contents of H register left through carry flag and store bit 7 in CF, flags updated
            mem.reg_rl(RH, false); // Carry = false (Rotate left with carry RL H)
            add_ticks(8);
            break;
        case 0x15:  // Rotate contents of L register left through carry flag and store bit 7 in CF, flags updated
            mem.reg_rl(RL, false); // Carry = false (Rotate left with carry RL L)
            add_ticks(8);
            break;
        case 0x16:  // Rotate contents of byte at (HL) lef through carry flag and store bit 7 in CF, flags updated
            mem.rl_from_pointer(Register16Bit::HL, false); // Carry = false (Rotate left with carry RL L)
            add_ticks(16);
            break;
        case 0x0f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            mem.reg_rr(RA, true); // Carry = true (Rotate right with carry RRC A)
            add_ticks(8);
            break;
        case 0x08:  // Rotate contents of B register right and store bit 0 in CF, flags updated
            mem.reg_rr(RB, true); // Carry = true (Rotate right with carry RRC B)
            add_ticks(8);
            break;
        case 0x09:  // Rotate contents of C register right and store bit 0 in CF, flags updated
            mem.reg_rr(RC, true); // Carry = true (Rotate right with carry RRC C)
            add_ticks(8);
            break;
        case 0x0a:  // Rotate contents of D register right and store bit 0 in CF, flags updated
            mem.reg_rr(RD, true); // Carry = true (Rotate right with carry RRC D)
            add_ticks(8);
            break;
        case 0x0b:  // Rotate contents of E register right and store bit 0 in CF, flags updated
            mem.reg_rr(RE, true); // Carry = true (Rotate right with carry RRC E)
            add_ticks(8);
            break;
        case 0x0c:  // Rotate contents of H register right and store bit 0 in CF, flags updated
            mem.reg_rr(RH, true); // Carry = true (Rotate right with carry RRC H)
            add_ticks(8);
            break;
        case 0x0d:  // Rotate contents of L register right and store bit 0 in CF, flags updated
            mem.reg_rr(RL, true); // Carry = true (Rotate right with carry RRC L)
            add_ticks(8);
            break;
        case 0x0e:  // Rotate contents of byte at (HL) right and store bit 0 in CF, flags updated
            mem.rr_from_pointer(Register16Bit::HL, true); // Carry = true (Rotate right with carry RRC (HL))
            add_ticks(16);
            break;
        case 0x1f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            mem.reg_rr(RA, false); // Carry = false (Rotate right RR A)
            add_ticks(8);
            break;
        case 0x18:  // Rotate contents of B register right and store bit 0 in CF, flags updated
            mem.reg_rr(RB, false); // Carry = false (Rotate right with carry RR B)
            add_ticks(8);
            break;
        case 0x19:  // Rotate contents of C register right and store bit 0 in CF, flags updated
            mem.reg_rr(RC, false); // Carry = false (Rotate right with carry RR C)
            add_ticks(8);
            break;
        case 0x1a:  // Rotate contents of D register right and store bit 0 in CF, flags updated
            mem.reg_rr(RD, false); // Carry = false (Rotate right with carry RR D)
            add_ticks(8);
            break;
        case 0x1b:  // Rotate contents of E register right and store bit 0 in CF, flags updated
            mem.reg_rr(RE, false); // Carry = false (Rotate right with carry RR E)
            add_ticks(8);
            break;
        case 0x1c:  // Rotate contents of H register right and store bit 0 in CF, flags updated
            mem.reg_rr(RH, false); // Carry = false (Rotate right with carry RR H)
            add_ticks(8);
            break;
        case 0x1d:  // Rotate contents of L register right and store bit 0 in CF, flags updated
            mem.reg_rr(RL, false); // Carry = false (Rotate right with carry RR L)
            add_ticks(8);
            break;
        case 0x1e:  // Rotate contents of byte at (HL) right and store bit 0 in CF, flags updated
            mem.rr_from_pointer(Register16Bit::HL, false); // Carry = false (Rotate right with carry RR (HL))
            add_ticks(16);
            break;
        case 0x27:  // Shift contents of register A left and store bit 7 in CF, bit0 = 0, flags updated
            mem.reg_sla(RA);
            add_ticks(8);
            break;
        case 0x20:  // Shift contents of register B left and store bit 7 in CF, bit0 = 0, flags updated
            mem.reg_sla(RB);
            add_ticks(8);
            break;
        case 0x21:  // Shift contents of register C left and store bit 7 in CF, bit0 = 0, flags updated
            mem.reg_sla(RC);
            add_ticks(8);
            break;
        case 0x22:  // Shift contents of register D left and store bit 7 in CF, bit0 = 0, flags updated
            mem.reg_sla(RD);
            add_ticks(8);
            break;
        case 0x23:  // Shift contents of register E left and store bit 7 in CF, bit0 = 0, flags updated
            mem.reg_sla(RE);
            add_ticks(8);
            break;
        case 0x24:  // Shift contents of register H left and store bit 7 in CF, bit0 = 0, flags updated
            mem.reg_sla(RH);
            add_ticks(8);
            break;
        case 0x25:  // Shift contents of register L left and store bit 7 in CF, bit0 = 0, flags updated
            mem.reg_sla(RL);
            add_ticks(8);
            break;
        case 0x26:  // Shift contents of byte at (HL) left and store bit 7 in CF, bit0 = 0, flags updated
            mem.sla_from_pointer(Register16Bit::HL);
            add_ticks(16);
            break;
        case 0x2f:  // Shift contents of register A right and store bit 0 in CF, bit7 unchanged, flags updated
            mem.reg_sra(RA);
            add_ticks(8);
            break;
        case 0x28:  // Shift contents of register B right and store bit 0 in CF, bit7 unchanged, flags updated
            mem.reg_sra(RB);
            add_ticks(8);
            break;
        case 0x29:  // Shift contents of register C right and store bit 0 in CF, bit7 unchanged, flags updated
            mem.reg_sra(RC);
            add_ticks(8);
            break;
        case 0x2a:  // Shift contents of register D right and store bit 0 in CF, bit7 unchanged, flags updated
            mem.reg_sra(RD);
            add_ticks(8);
            break;
        case 0x2b:  // Shift contents of register E right and store bit 0 in CF, bit7 unchanged, flags updated
            mem.reg_sra(RE);
            add_ticks(8);
            break;
        case 0x2c:  // Shift contents of register H right and store bit 0 in CF, bit7 unchanged, flags updated
            mem.reg_sra(RH);
            add_ticks(8);
            break;
        case 0x2d:  // Shift contents of register L right and store bit 0 in CF, bit7 unchanged, flags updated
            mem.reg_sra(RL);
            add_ticks(8);
            break;
        case 0x2e:  // Shift contents byte at (HL) right and store bit 0 in CF, bit7 unchanged, flags updated
            mem.sra_from_pointer(Register16Bit::HL);
            add_ticks(16);
            break;
        case 0x3f:  // Shift contents of register A right and store bit 0 in CF, bit7 = 0, flags updated
            mem.reg_srl(RA);
            add_ticks(8);
            break;
        case 0x38:  // Shift contents of register B right and store bit 0 in CF, bit7 = 0, flags updated
            mem.reg_srl(RB);
            add_ticks(8);
            break;
        case 0x39:  // Shift contents of register C right and store bit 0 in CF, bit7 = 0, flags updated
            mem.reg_srl(RC);
            add_ticks(8);
            break;
        case 0x3a:  // Shift contents of register D right and store bit 0 in CF, bit7 = 0, flags updated
            mem.reg_srl(RD);
            add_ticks(8);
            break;
        case 0x3b:  // Shift contents of register E right and store bit 0 in CF, bit7 = 0, flags updated
            mem.reg_srl(RE);
            add_ticks(8);
            break;
        case 0x3c:  // Shift contents of register H right and store bit 0 in CF, bit7 = 0, flags updated
            mem.reg_srl(RH);
            add_ticks(8);
            break;
        case 0x3d:  // Shift contents of register L right and store bit 0 in CF, bit7 = 0, flags updated
            mem.reg_srl(RL);
            add_ticks(8);
            break;
        case 0x3e:  // Shift contents byte at (HL) right and store bit 0 in CF, bit7 = 0, flags updated
            mem.srl_from_pointer(Register16Bit::HL);
            add_ticks(16);
            break;
        case 0x47:  // Test bit 0 in register A and set flags accordingly
            mem.bit_test(RA, 0);
            add_ticks(8);
            break;
        case 0x40:  // Test bit 0 in register B and set flags accordingly
            mem.bit_test(RB, 0);
            add_ticks(8);
            break;
        case 0x41:  // Test bit 0 in register C and set flags accordingly
            mem.bit_test(RC, 0);
            add_ticks(8);
            break;
        case 0x42:  // Test bit 0 in register D and set flags accordingly
            mem.bit_test(RD, 0);
            add_ticks(8);
            break;
        case 0x43:  // Test bit 0 in register E and set flags accordingly
            mem.bit_test(RE, 0);
            add_ticks(8);
            break;
        case 0x44:  // Test bit 0 in register H and set flags accordingly
            mem.bit_test(RH, 0);
            add_ticks(8);
            break;
        case 0x45:  // Test bit 0 in register L and set flags accordingly
            mem.bit_test(RL, 0);
            add_ticks(8);
            break;
        case 0x46:  // Test bit 0 in byte at (HL) and set flags accordingly
            mem.bit_test_from_pointer(Register16Bit::HL, 0);
            add_ticks(12);
            break;
        case 0x4f:  // Test bit 1 in register A and set flags accordingly
            mem.bit_test(RA, 1);
            add_ticks(8);
            break;
        case 0x48:  // Test bit 1 in register B and set flags accordingly
            mem.bit_test(RB, 1);
            add_ticks(8);
            break;
        case 0x49:  // Test bit 1 in register C and set flags accordingly
            mem.bit_test(RC, 1);
            add_ticks(8);
            break;
        case 0x4a:  // Test bit 1 in register D and set flags accordingly
            mem.bit_test(RD, 1);
            add_ticks(8);
            break;
        case 0x4b:  // Test bit 1 in register E and set flags accordingly
            mem.bit_test(RE, 1);
            add_ticks(8);
            break;
        case 0x4c:  // Test bit 1 in register H and set flags accordingly
            mem.bit_test(RH, 1);
            add_ticks(8);
            break;
        case 0x4d:  // Test bit 1 in register L and set flags accordingly
            mem.bit_test(RL, 1);
            add_ticks(8);
            break;
        case 0x4e:  // Test bit 1 in byte at (HL) and set flags accordingly
            mem.bit_test_from_pointer(Register16Bit::HL, 1);
            add_ticks(12);
            break;
        case 0x57:  // Test bit 2 in register A and set flags accordingly
            mem.bit_test(RA, 2);
            add_ticks(8);
            break;
        case 0x50:  // Test bit 2 in register B and set flags accordingly
            mem.bit_test(RB, 2);
            add_ticks(8);
            break;
        case 0x51:  // Test bit 2 in register C and set flags accordingly
            mem.bit_test(RC, 2);
            add_ticks(8);
            break;
        case 0x52:  // Test bit 2 in register D and set flags accordingly
            mem.bit_test(RD, 2);
            add_ticks(8);
            break;
        case 0x53:  // Test bit 2 in register E and set flags accordingly
            mem.bit_test(RE, 2);
            add_ticks(8);
            break;
        case 0x54:  // Test bit 2 in register H and set flags accordingly
            mem.bit_test(RH, 2);
            add_ticks(8);
            break;
        case 0x55:  // Test bit 2 in register L and set flags accordingly
            mem.bit_test(RL, 2);
            add_ticks(8);
            break;
        case 0x56:  // Test bit 2 in byte at (HL) and set flags accordingly
            mem.bit_test_from_pointer(Register16Bit::HL, 2);
            add_ticks(12);
            break;
        case 0x5f:  // Test bit 3 in register A and set flags accordingly
            mem.bit_test(RA, 3);
            add_ticks(8);
            break;
        case 0x58:  // Test bit 3 in register B and set flags accordingly
            mem.bit_test(RB, 3);
            add_ticks(8);
            break;
        case 0x59:  // Test bit 3 in register C and set flags accordingly
            mem.bit_test(RC, 3);
            add_ticks(8);
            break;
        case 0x5a:  // Test bit 3 in register D and set flags accordingly
            mem.bit_test(RD, 3);
            add_ticks(8);
            break;
        case 0x5b:  // Test bit 3 in register E and set flags accordingly
            mem.bit_test(RE, 3);
            add_ticks(8);
            break;
        case 0x5c:  // Test bit 3 in register H and set flags accordingly
            mem.bit_test(RH, 3);
            add_ticks(8);
            break;
        case 0x5d:  // Test bit 3 in register L and set flags accordingly
            mem.bit_test(RL, 3);
            add_ticks(8);
            break;
        case 0x5e:  // Test bit 3 in byte at (HL) and set flags accordingly
            mem.bit_test_from_pointer(Register16Bit::HL, 3);
            add_ticks(12);
            break;
        case 0x67:  // Test bit 4 in register A and set flags accordingly
            mem.bit_test(RA, 4);
            add_ticks(8);
            break;
        case 0x60:  // Test bit 4 in register B and set flags accordingly
            mem.bit_test(RB, 4);
            add_ticks(8);
            break;
        case 0x61:  // Test bit 4 in register C and set flags accordingly
            mem.bit_test(RC, 4);
            add_ticks(8);
            break;
        case 0x62:  // Test bit 4 in register D and set flags accordingly
            mem.bit_test(RD, 4);
            add_ticks(8);
            break;
        case 0x63:  // Test bit 4 in register E and set flags accordingly
            mem.bit_test(RE, 4);
            add_ticks(8);
            break;
        case 0x64:  // Test bit 4 in register H and set flags accordingly
            mem.bit_test(RH, 4);
            add_ticks(8);
            break;
        case 0x65:  // Test bit 4 in register L and set flags accordingly
            mem.bit_test(RL, 4);
            add_ticks(8);
            break;
        case 0x66:  // Test bit 4 in byte at (HL) and set flags accordingly
            mem.bit_test_from_pointer(Register16Bit::HL, 4);
            add_ticks(12);
            break;
        case 0x6f:  // Test bit 5 in register A and set flags accordingly
            mem.bit_test(RA, 5);
            add_ticks(8);
            break;
        case 0x68:  // Test bit 5 in register B and set flags accordingly
            mem.bit_test(RB, 5);
            add_ticks(8);
            break;
        case 0x69:  // Test bit 5 in register C and set flags accordingly
            mem.bit_test(RC, 5);
            add_ticks(8);
            break;
        case 0x6a:  // Test bit 5 in register D and set flags accordingly
            mem.bit_test(RD, 5);
            add_ticks(8);
            break;
        case 0x6b:  // Test bit 5 in register E and set flags accordingly
            mem.bit_test(RE, 5);
            add_ticks(8);
            break;
        case 0x6c:  // Test bit 5 in register H and set flags accordingly
            mem.bit_test(RH, 5);
            add_ticks(8);
            break;
        case 0x6d:  // Test bit 5 in register L and set flags accordingly
            mem.bit_test(RL, 5);
            add_ticks(8);
            break;
        case 0x6e:  // Test bit 5 in byte at (HL) and set flags accordingly
            mem.bit_test_from_pointer(Register16Bit::HL, 5);
            add_ticks(12);
            break;
        case 0x77:  // Test bit 6 in register A and set flags accordingly
            mem.bit_test(RA, 6);
            add_ticks(8);
            break;
        case 0x70:  // Test bit 6 in register B and set flags accordingly
            mem.bit_test(RB, 6);
            add_ticks(8);
            break;
        case 0x71:  // Test bit 6 in register C and set flags accordingly
            mem.bit_test(RC, 6);
            add_ticks(8);
            break;
        case 0x72:  // Test bit 6 in register D and set flags accordingly
            mem.bit_test(RD, 6);
            add_ticks(8);
            break;
        case 0x73:  // Test bit 6 in register E and set flags accordingly
            mem.bit_test(RE, 6);
            add_ticks(8);
            break;
        case 0x74:  // Test bit 6 in register H and set flags accordingly
            mem.bit_test(RH, 6);
            add_ticks(8);
            break;
        case 0x75:  // Test bit 6 in register L and set flags accordingly
            mem.bit_test(RL, 6);
            add_ticks(8);
            break;
        case 0x76:  // Test bit 6 in byte at (HL) and set flags accordingly
            mem.bit_test_from_pointer(Register16Bit::HL, 6);
            add_ticks(12);
            break;
        case 0x7f:  // Test bit 7 in register A and set flags accordingly
            mem.bit_test(RA, 7);
            add_ticks(8);
            break;
        case 0x78:  // Test bit 7 in register B and set flags accordingly
            mem.bit_test(RB, 7);
            add_ticks(8);
            break;
        case 0x79:  // Test bit 7 in register C and set flags accordingly
            mem.bit_test(RC, 7);
            add_ticks(8);
            break;
        case 0x7a:  // Test bit 7 in register D and set flags accordingly
            mem.bit_test(RD, 7);
            add_ticks(8);
            break;
        case 0x7b:  // Test bit 7 in register E and set flags accordingly
            mem.bit_test(RE, 7);
            add_ticks(8);
            break;
        case 0x7c:  // Test bit 7 in register H and set flags accordingly
            mem.bit_test(RH, 7);
            add_ticks(8);
            break;
        case 0x7d:  // Test bit 7 in register L and set flags accordingly
            mem.bit_test(RL, 7);
            add_ticks(8);
            break;
        case 0x7e:  // Test bit 7 in byte at (HL) and set flags accordingly
            mem.bit_test_from_pointer(Register16Bit::HL, 7);
            add_ticks(12);
            break;
        case 0xc7:  // Set bit 0 in register A, flags not affected
            mem.bit_set(RA, 0);
            add_ticks(8);
            break;
        case 0xc0:  // Set bit 0 in register B, flags not affected
            mem.bit_set(RB, 0);
            add_ticks(8);
            break;
        case 0xc1:  // Set bit 0 in register C, flags not affected
            mem.bit_set(RC, 0);
            add_ticks(8);
            break;
        case 0xc2:  // Set bit 0 in register D, flags not affectedy
            mem.bit_set(RD, 0);
            add_ticks(8);
            break;
        case 0xc3:  // Set bit 0 in register E, flags not affected
            mem.bit_set(RE, 0);
            add_ticks(8);
            break;
        case 0xc4:  // Set bit 0 in register H, flags not affected
            mem.bit_set(RH, 0);
            add_ticks(8);
            break;
        case 0xc5:  // Set bit 0 in register L, flags not affected
            mem.bit_set(RL, 0);
            add_ticks(8);
            break;
        case 0xc6:  // Set bit 0 in byte at (HL), flags not affected
            mem.bit_set_from_pointer(Register16Bit::HL, 0);
            add_ticks(16);
            break;
        case 0xcf:  // Set bit 1 in register A, flags not affected
            mem.bit_set(RA, 1);
            add_ticks(8);
            break;
        case 0xc8:  // Set bit 1 in register B, flags not affected
            mem.bit_set(RB, 1);
            add_ticks(8);
            break;
        case 0xc9:  // Set bit 1 in register C, flags not affected
            mem.bit_set(RC, 1);
            add_ticks(8);
            break;
        case 0xca:  // Set bit 1 in register D, flags not affectedy
            mem.bit_set(RD, 1);
            add_ticks(8);
            break;
        case 0xcb:  // Set bit 1 in register E, flags not affected
            mem.bit_set(RE, 1);
            add_ticks(8);
            break;
        case 0xcc:  // Set bit 1 in register H, flags not affected
            mem.bit_set(RH, 1);
            add_ticks(8);
            break;
        case 0xcd:  // Set bit 1 in register L, flags not affected
            mem.bit_set(RL, 1);
            add_ticks(8);
            break;
        case 0xce:  // Set bit 1 in byte at (HL), flags not affected
            mem.bit_set_from_pointer(Register16Bit::HL, 1);
            add_ticks(16);
            break;
        case 0xd7:  // Set bit 2 in register A, flags not affected
            mem.bit_set(RA, 2);
            add_ticks(8);
            break;
        case 0xd0:  // Set bit 2 in register B, flags not affected
            mem.bit_set(RB, 2);
            add_ticks(8);
            break;
        case 0xd1:  // Set bit 2 in register C, flags not affected
            mem.bit_set(RC, 2);
            add_ticks(8);
            break;
        case 0xd2:  // Set bit 2 in register D, flags not affectedy
            mem.bit_set(RD, 2);
            add_ticks(8);
            break;
        case 0xd3:  // Set bit 2 in register E, flags not affected
            mem.bit_set(RE, 2);
            add_ticks(8);
            break;
        case 0xd4:  // Set bit 2 in register H, flags not affected
            mem.bit_set(RH, 2);
            add_ticks(8);
            break;
        case 0xd5:  // Set bit 2 in register L, flags not affected
            mem.bit_set(RL, 2);
            add_ticks(8);
            break;
        case 0xd6:  // Set bit 2 in byte at (HL), flags not affected
            mem.bit_set_from_pointer(Register16Bit::HL, 2);
            add_ticks(16);
            break;
        case 0xdf:  // Set bit 3 in register A, flags not affected
            mem.bit_set(RA, 3);
            add_ticks(8);
            break;
        case 0xd8:  // Set bit 3 in register B, flags not affected
            mem.bit_set(RB, 3);
            add_ticks(8);
            break;
        case 0xd9:  // Set bit 3 in register C, flags not affected
            mem.bit_set(RC, 3);
            add_ticks(8);
            break;
        case 0xda:  // Set bit 3 in register D, flags not affectedy
            mem.bit_set(RD, 3);
            add_ticks(8);
            break;
        case 0xdb:  // Set bit 3 in register E, flags not affected
            mem.bit_set(RE, 3);
            add_ticks(8);
            break;
        case 0xdc:  // Set bit 3 in register H, flags not affected
            mem.bit_set(RH, 3);
            add_ticks(8);
            break;
        case 0xdd:  // Set bit 3 in register L, flags not affected
            mem.bit_set(RL, 3);
            add_ticks(8);
            break;
        case 0xde:  // Set bit 3 in byte at (HL), flags not affected
            mem.bit_set_from_pointer(Register16Bit::HL, 3);
            add_ticks(16);
            break;
        case 0xe7:  // Set bit 4 in register A, flags not affected
            mem.bit_set(RA, 4);
            add_ticks(8);
            break;
        case 0xe0:  // Set bit 4 in register B, flags not affected
            mem.bit_set(RB, 4);
            add_ticks(8);
            break;
        case 0xe1:  // Set bit 4 in register C, flags not affected
            mem.bit_set(RC, 4);
            add_ticks(8);
            break;
        case 0xe2:  // Set bit 4 in register D, flags not affectedy
            mem.bit_set(RD, 4);
            add_ticks(8);
            break;
        case 0xe3:  // Set bit 4 in register E, flags not affected
            mem.bit_set(RE, 4);
            add_ticks(8);
            break;
        case 0xe4:  // Set bit 4 in register H, flags not affected
            mem.bit_set(RH, 4);
            add_ticks(8);
            break;
        case 0xe5:  // Set bit 4 in register L, flags not affected
            mem.bit_set(RL, 4);
            add_ticks(8);
            break;
        case 0xe6:  // Set bit 4 in byte at (HL), flags not affected
            mem.bit_set_from_pointer(Register16Bit::HL, 4);
            add_ticks(16);
            break;
        case 0xef:  // Set bit 5 in register A, flags not affected
            mem.bit_set(RA, 5);
            add_ticks(8);
            break;
        case 0xe8:  // Set bit 5 in register B, flags not affected
            mem.bit_set(RB, 5);
            add_ticks(8);
            break;
        case 0xe9:  // Set bit 5 in register C, flags not affected
            mem.bit_set(RC, 5);
            add_ticks(8);
            break;
        case 0xea:  // Set bit 5 in register D, flags not affectedy
            mem.bit_set(RD, 5);
            add_ticks(8);
            break;
        case 0xeb:  // Set bit 5 in register E, flags not affected
            mem.bit_set(RE, 5);
            add_ticks(8);
            break;
        case 0xec:  // Set bit 5 in register H, flags not affected
            mem.bit_set(RH, 5);
            add_ticks(8);
            break;
        case 0xed:  // Set bit 5 in register L, flags not affected
            mem.bit_set(RL, 5);
            add_ticks(8);
            break;
        case 0xee:  // Set bit 5 in byte at (HL), flags not affected
            mem.bit_set_from_pointer(Register16Bit::HL, 5);
            add_ticks(16);
            break;
       case 0xf7:  // Set bit 6 in register A, flags not affected
            mem.bit_set(RA, 6);
            add_ticks(8);
            break;
        case 0xf0:  // Set bit 6 in register B, flags not affected
            mem.bit_set(RB, 6);
            add_ticks(8);
            break;
        case 0xf1:  // Set bit 6 in register C, flags not affected
            mem.bit_set(RC, 6);
            add_ticks(8);
            break;
        case 0xf2:  // Set bit 6 in register D, flags not affectedy
            mem.bit_set(RD, 6);
            add_ticks(8);
            break;
        case 0xf3:  // Set bit 6 in register E, flags not affected
            mem.bit_set(RE, 6);
            add_ticks(8);
            break;
        case 0xf4:  // Set bit 6 in register H, flags not affected
            mem.bit_set(RH, 6);
            add_ticks(8);
            break;
        case 0xf5:  // Set bit 6 in register L, flags not affected
            mem.bit_set(RL, 6);
            add_ticks(8);
            break;
        case 0xf6:  // Set bit 6 in byte at (HL), flags not affected
            mem.bit_set_from_pointer(Register16Bit::HL, 6);
            add_ticks(16);
            break;
        case 0xff:  // Set bit 7 in register A, flags not affected
            mem.bit_set(RA, 7);
            add_ticks(8);
            break;
        case 0xf8:  // Set bit 7 in register B, flags not affected
            mem.bit_set(RB, 7);
            add_ticks(8);
            break;
        case 0xf9:  // Set bit 7 in register C, flags not affected
            mem.bit_set(RC, 7);
            add_ticks(8);
            break;
        case 0xfa:  // Set bit 7 in register D, flags not affectedy
            mem.bit_set(RD, 7);
            add_ticks(8);
            break;
        case 0xfb:  // Set bit 7 in register E, flags not affected
            mem.bit_set(RE, 7);
            add_ticks(8);
            break;
        case 0xfc:  // Set bit 7 in register H, flags not affected
            mem.bit_set(RH, 7);
            add_ticks(8);
            break;
        case 0xfd:  // Set bit 7 in register L, flags not affected
            mem.bit_set(RL, 7);
            add_ticks(8);
            break;
        case 0xfe:  // Set bit 7 in byte at (HL), flags not affected
            mem.bit_set_from_pointer(Register16Bit::HL, 7);
            add_ticks(16);
            break;
        case 0x87:  // Reset bit 0 in register A, flags not affected
            mem.bit_res(RA, 0);
            add_ticks(8);
            break;
        case 0x80:  // Reset bit 0 in register B, flags not affected
            mem.bit_res(RB, 0);
            add_ticks(8);
            break;
        case 0x81:  // Reset bit 0 in register C, flags not affected
            mem.bit_res(RC, 0);
            add_ticks(8);
            break;
        case 0x82:  // Reset bit 0 in register D, flags not affectedy
            mem.bit_res(RD, 0);
            add_ticks(8);
            break;
        case 0x83:  // Reset bit 0 in register E, flags not affected
            mem.bit_res(RE, 0);
            add_ticks(8);
            break;
        case 0x84:  // Reset bit 0 in register H, flags not affected
            mem.bit_res(RH, 0);
            add_ticks(8);
            break;
        case 0x85:  // Reset bit 0 in register L, flags not affected
            mem.bit_res(RL, 0);
            add_ticks(8);
            break;
        case 0x86:  // Reset bit 0 in byte at (HL), flags not affected
            mem.bit_res_from_pointer(Register16Bit::HL, 0);
            add_ticks(16);
            break;
        case 0x8f:  // Reset bit 1 in register A, flags not affected
            mem.bit_res(RA, 1);
            add_ticks(8);
            break;
        case 0x88:  // Reset bit 1 in register B, flags not affected
            mem.bit_res(RB, 1);
            add_ticks(8);
            break;
        case 0x89:  // Reset bit 1 in register C, flags not affected
            mem.bit_res(RC, 1);
            add_ticks(8);
            break;
        case 0x8a:  // Reset bit 1 in register D, flags not affectedy
            mem.bit_res(RD, 1);
            add_ticks(8);
            break;
        case 0x8b:  // Reset bit 1 in register E, flags not affected
            mem.bit_res(RE, 1);
            add_ticks(8);
            break;
        case 0x8c:  // Reset bit 1 in register H, flags not affected
            mem.bit_res(RH, 1);
            add_ticks(8);
            break;
        case 0x8d:  // Reset bit 1 in register L, flags not affected
            mem.bit_res(RL, 1);
            add_ticks(8);
            break;
        case 0x8e:  // Reset bit 1 in byte at (HL), flags not affected
            mem.bit_res_from_pointer(Register16Bit::HL, 1);
            add_ticks(16);
            break;
        case 0x97:  // Reset bit 2 in register A, flags not affected
            mem.bit_res(RA, 2);
            add_ticks(8);
            break;
        case 0x90:  // Reset bit 2 in register B, flags not affected
            mem.bit_res(RB, 2);
            add_ticks(8);
            break;
        case 0x91:  // Reset bit 2 in register C, flags not affected
            mem.bit_res(RC, 2);
            add_ticks(8);
            break;
        case 0x92:  // Reset bit 2 in register D, flags not affectedy
            mem.bit_res(RD, 2);
            add_ticks(8);
            break;
        case 0x93:  // Reset bit 2 in register E, flags not affected
            mem.bit_res(RE, 2);
            add_ticks(8);
            break;
        case 0x94:  // Reset bit 2 in register H, flags not affected
            mem.bit_res(RH, 2);
            add_ticks(8);
            break;
        case 0x95:  // Reset bit 2 in register L, flags not affected
            mem.bit_res(RL, 2);
            add_ticks(8);
            break;
        case 0x96:  // Reset bit 2 in byte at (HL), flags not affected
            mem.bit_res_from_pointer(Register16Bit::HL, 2);
            add_ticks(16);
            break;
        case 0x9f:  // Reset bit 3 in register A, flags not affected
            mem.bit_res(RA, 3);
            add_ticks(8);
            break;
        case 0x98:  // Reset bit 3 in register B, flags not affected
            mem.bit_res(RB, 3);
            add_ticks(8);
            break;
        case 0x99:  // Reset bit 3 in register C, flags not affected
            mem.bit_res(RC, 3);
            add_ticks(8);
            break;
        case 0x9a:  // Reset bit 3 in register D, flags not affectedy
            mem.bit_res(RD, 3);
            add_ticks(8);
            break;
        case 0x9b:  // Reset bit 3 in register E, flags not affected
            mem.bit_res(RE, 3);
            add_ticks(8);
            break;
        case 0x9c:  // Reset bit 3 in register H, flags not affected
            mem.bit_res(RH, 3);
            add_ticks(8);
            break;
        case 0x9d:  // Reset bit 3 in register L, flags not affected
            mem.bit_res(RL, 3);
            add_ticks(8);
            break;
        case 0x9e:  // Reset bit 3 in byte at (HL), flags not affected
            mem.bit_res_from_pointer(Register16Bit::HL, 3);
            add_ticks(16);
            break;
        case 0xa7:  // Reset bit 4 in register A, flags not affected
            mem.bit_res(RA, 4);
            add_ticks(8);
            break;
        case 0xa0:  // Reset bit 4 in register B, flags not affected
            mem.bit_res(RB, 4);
            add_ticks(8);
            break;
        case 0xa1:  // Reset bit 4 in register C, flags not affected
            mem.bit_res(RC, 4);
            add_ticks(8);
            break;
        case 0xa2:  // Reset bit 4 in register D, flags not affectedy
            mem.bit_res(RD, 4);
            add_ticks(8);
            break;
        case 0xa3:  // Reset bit 4 in register E, flags not affected
            mem.bit_res(RE, 4);
            add_ticks(8);
            break;
        case 0xa4:  // Reset bit 4 in register H, flags not affected
            mem.bit_res(RH, 4);
            add_ticks(8);
            break;
        case 0xa5:  // Reset bit 4 in register L, flags not affected
            mem.bit_res(RL, 4);
            add_ticks(8);
            break;
        case 0xa6:  // Reset bit 4 in byte at (HL), flags not affected
            mem.bit_res_from_pointer(Register16Bit::HL, 4);
            add_ticks(16);
            break;
        case 0xaf:  // Reset bit 5 in register A, flags not affected
            mem.bit_res(RA, 5);
            add_ticks(8);
            break;
        case 0xa8:  // Reset bit 5 in register B, flags not affected
            mem.bit_res(RB, 5);
            add_ticks(8);
            break;
        case 0xa9:  // Reset bit 5 in register C, flags not affected
            mem.bit_res(RC, 5);
            add_ticks(8);
            break;
        case 0xaa:  // Reset bit 5 in register D, flags not affectedy
            mem.bit_res(RD, 5);
            add_ticks(8);
            break;
        case 0xab:  // Reset bit 5 in register E, flags not affected
            mem.bit_res(RE, 5);
            add_ticks(8);
            break;
        case 0xac:  // Reset bit 5 in register H, flags not affected
            mem.bit_res(RH, 5);
            add_ticks(8);
            break;
        case 0xad:  // Reset bit 5 in register L, flags not affected
            mem.bit_res(RL, 5);
            add_ticks(8);
            break;
        case 0xae:  // Reset bit 5 in byte at (HL), flags not affected
            mem.bit_res_from_pointer(Register16Bit::HL, 5);
            add_ticks(16);
            break;
        case 0xb7:  // Reset bit 6 in register A, flags not affected
            mem.bit_res(RA, 6);
            add_ticks(8);
            break;
        case 0xb0:  // Reset bit 6 in register B, flags not affected
            mem.bit_res(RB, 6);
            add_ticks(8);
            break;
        case 0xb1:  // Reset bit 6 in register C, flags not affected
            mem.bit_res(RC, 6);
            add_ticks(8);
            break;
        case 0xb2:  // Reset bit 6 in register D, flags not affectedy
            mem.bit_res(RD, 6);
            add_ticks(8);
            break;
        case 0xb3:  // Reset bit 6 in register E, flags not affected
            mem.bit_res(RE, 6);
            add_ticks(8);
            break;
        case 0xb4:  // Reset bit 6 in register H, flags not affected
            mem.bit_res(RH, 6);
            add_ticks(8);
            break;
        case 0xb5:  // Reset bit 6 in register L, flags not affected
            mem.bit_res(RL, 6);
            add_ticks(8);
            break;
        case 0xb6:  // Reset bit 6 in byte at (HL), flags not affected
            mem.bit_res_from_pointer(Register16Bit::HL, 6);
            add_ticks(16);
            break;
        case 0xbf:  // Reset bit 7 in register A, flags not affected
            mem.bit_res(RA, 7);
            add_ticks(8);
            break;
        case 0xb8:  // Reset bit 7 in register B, flags not affected
            mem.bit_res(RB, 7);
            add_ticks(8);
            break;
        case 0xb9:  // Reset bit 7 in register C, flags not affected
            mem.bit_res(RC, 7);
            add_ticks(8);
            break;
        case 0xba:  // Reset bit 7 in register D, flags not affectedy
            mem.bit_res(RD, 7);
            add_ticks(8);
            break;
        case 0xbb:  // Reset bit 7 in register E, flags not affected
            mem.bit_res(RE, 7);
            add_ticks(8);
            break;
        case 0xbc:  // Reset bit 7 in register H, flags not affected
            mem.bit_res(RH, 7);
            add_ticks(8);
            break;
        case 0xbd:  // Reset bit 7 in register L, flags not affected
            mem.bit_res(RL, 7);
            add_ticks(8);
            break;
        case 0xbe:  // Reset bit 7 in byte at (HL), flags not affected
            mem.bit_res_from_pointer(Register16Bit::HL, 7);
            add_ticks(16);
            break;
    }
}

// Halt the CPU until an interrupt occurs (HALT)
void Cpu::halt()
{
    halt_flag = true;
}

// Stop the CPU and LCD until a button is pressed (STOP)
void Cpu::stop()
{
    stop_flag = true;
}

// Exit stopped status
void Cpu::cancel_stop()
{
    stop_flag = false;
    add_ticks(217);
}