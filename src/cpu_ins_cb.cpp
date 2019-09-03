#include "cpu.h"
#include "memory.h"
#include "clock.h"

// Process a CB prefixed instruction
void Cpu::process_cb_instruction(uint8_t rom_byte)
{
    // switch statement to match opcode and perform operations
    switch(rom_byte)
    {
        case 0x37:  // Swap upper and lower nibbles of register A, flags updated
            mem->reg_swap(RA);
            clk->add_cycles(8);
            break;
        case 0x30:  // Swap upper and lower nibbles of register B, flags updated
            mem->reg_swap(RB);
            clk->add_cycles(8);
            break;
        case 0x31:  // Swap upper and lower nibbles of register C, flags updated
            mem->reg_swap(RC);
            clk->add_cycles(8);
            break;
        case 0x32:  // Swap upper and lower nibbles of register D, flags updated
            mem->reg_swap(RD);
            clk->add_cycles(8);
            break;
        case 0x33:  // Swap upper and lower nibbles of register E, flags updated
            mem->reg_swap(RE);
            clk->add_cycles(8);
            break;
        case 0x34:  // Swap upper and lower nibbles of register H, flags updated
            mem->reg_swap(RH);
            clk->add_cycles(8);
            break;
        case 0x35:  // Swap upper and lower nibbles of register L, flags updated
            mem->reg_swap(RL);
            clk->add_cycles(8);
            break;
        case 0x36:  // Swap upper and lower nibbles of byte at (HL), flags updated
            mem->swap_from_pointer(RHL);
            clk->add_cycles(16);
            break;
        case 0x07:  // Rotate contents of A register left and store bit 7 in CF, flags updated
            mem->reg_rl(RA, true); // Carry = true (Rotate left with carry RLC A)
            clk->add_cycles(8);
            break;
        case 0x00:  // Rotate contents of B register left and store bit 7 in CF, flags updated
            mem->reg_rl(RB, true); // Carry = true (Rotate left with carry RLC B)
            clk->add_cycles(8);
            break;
        case 0x01:  // Rotate contents of C register left and store bit 7 in CF, flags updated
            mem->reg_rl(RC, true); // Carry = true (Rotate left with carry RLC C)
            clk->add_cycles(8);
            break;
        case 0x02:  // Rotate contents of D register left and store bit 7 in CF, flags updated
            mem->reg_rl(RD, true); // Carry = true (Rotate left with carry RLC D)
            clk->add_cycles(8);
            break;
        case 0x03:  // Rotate contents of E register left and store bit 7 in CF, flags updated
            mem->reg_rl(RE, true); // Carry = true (Rotate left with carry RLC E)
            clk->add_cycles(8);
            break;
        case 0x04:  // Rotate contents of H register left and store bit 7 in CF, flags updated
            mem->reg_rl(RH, true); // Carry = true (Rotate left with carry RLC H)
            clk->add_cycles(8);
            break;
        case 0x05:  // Rotate contents of L register left and store bit 7 in CF, flags updated
            mem->reg_rl(RL, true); // Carry = true (Rotate left with carry RLC L)
            clk->add_cycles(8);
            break;
        case 0x06:  // Rotate contents of byte at (HL) left and store bit 7 in CF, flags updated
            mem->rl_from_pointer(RL, true); // Carry = true (Rotate left with carry RLC L)
            clk->add_cycles(16);
            break;
        case 0x17:  // Rotate contents of A register left and store bit 7 in CF, flags updated
            mem->reg_rl(RA, false); // Carry = false (Rotate left RL A)
            clk->add_cycles(8);
            break;
        case 0x10:  // Rotate contents of B register left and store bit 7 in CF, flags updated
            mem->reg_rl(RB, false); // Carry = false (Rotate left with carry RL B)
            clk->add_cycles(8);
            break;
        case 0x11:  // Rotate contents of C register left and store bit 7 in CF, flags updated
            mem->reg_rl(RC, false); // Carry = false (Rotate left with carry RL C)
            clk->add_cycles(8);
            break;
        case 0x12:  // Rotate contents of D register left and store bit 7 in CF, flags updated
            mem->reg_rl(RD, false); // Carry = false (Rotate left with carry RL D)
            clk->add_cycles(8);
            break;
        case 0x13:  // Rotate contents of E register left and store bit 7 in CF, flags updated
            mem->reg_rl(RE, false); // Carry = false (Rotate left with carry RL E)
            clk->add_cycles(8);
            break;
        case 0x14:  // Rotate contents of H register left and store bit 7 in CF, flags updated
            mem->reg_rl(RH, false); // Carry = false (Rotate left with carry RL H)
            clk->add_cycles(8);
            break;
        case 0x15:  // Rotate contents of L register left and store bit 7 in CF, flags updated
            mem->reg_rl(RL, false); // Carry = false (Rotate left with carry RL L)
            clk->add_cycles(8);
            break;
        case 0x16:  // Rotate contents of byte at (HL) left and store bit 7 in CF, flags updated
            mem->rl_from_pointer(RL, false); // Carry = false (Rotate left with carry RL L)
            clk->add_cycles(16);
            break;
        case 0x0f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            mem->reg_rr(RA, true); // Carry = true (Rotate right with carry RRC A)
            clk->add_cycles(8);
            break;
        case 0x08:  // Rotate contents of B register right and store bit 0 in CF, flags updated
            mem->reg_rr(RB, true); // Carry = true (Rotate right with carry RRC B)
            clk->add_cycles(8);
            break;
        case 0x09:  // Rotate contents of C register right and store bit 0 in CF, flags updated
            mem->reg_rr(RC, true); // Carry = true (Rotate right with carry RRC C)
            clk->add_cycles(8);
            break;
        case 0x0a:  // Rotate contents of D register right and store bit 0 in CF, flags updated
            mem->reg_rr(RD, true); // Carry = true (Rotate right with carry RRC D)
            clk->add_cycles(8);
            break;
        case 0x0b:  // Rotate contents of E register right and store bit 0 in CF, flags updated
            mem->reg_rr(RE, true); // Carry = true (Rotate right with carry RRC E)
            clk->add_cycles(8);
            break;
        case 0x0c:  // Rotate contents of H register right and store bit 0 in CF, flags updated
            mem->reg_rr(RH, true); // Carry = true (Rotate right with carry RRC H)
            clk->add_cycles(8);
            break;
        case 0x0d:  // Rotate contents of L register right and store bit 0 in CF, flags updated
            mem->reg_rr(RL, true); // Carry = true (Rotate right with carry RRC L)
            clk->add_cycles(8);
            break;
        case 0x0e:  // Rotate contents of byte at (HL) right and store bit 0 in CF, flags updated
            mem->rr_from_pointer(RL, true); // Carry = true (Rotate right with carry RRC L)
            clk->add_cycles(16);
            break;
        case 0x1f:  // Rotate contents of A register right and store bit 0 in CF, flags updated
            mem->reg_rr(RA, false); // Carry = false (Rotate right RL A)
            clk->add_cycles(8);
            break;
        case 0x18:  // Rotate contents of B register right and store bit 0 in CF, flags updated
            mem->reg_rr(RB, false); // Carry = false (Rotate right with carry RL B)
            clk->add_cycles(8);
            break;
        case 0x19:  // Rotate contents of C register right and store bit 0 in CF, flags updated
            mem->reg_rr(RC, false); // Carry = false (Rotate right with carry RL C)
            clk->add_cycles(8);
            break;
        case 0x1a:  // Rotate contents of D register right and store bit 0 in CF, flags updated
            mem->reg_rr(RD, false); // Carry = false (Rotate right with carry RL D)
            clk->add_cycles(8);
            break;
        case 0x1b:  // Rotate contents of E register right and store bit 0 in CF, flags updated
            mem->reg_rr(RE, false); // Carry = false (Rotate right with carry RL E)
            clk->add_cycles(8);
            break;
        case 0x1c:  // Rotate contents of H register right and store bit 0 in CF, flags updated
            mem->reg_rr(RH, false); // Carry = false (Rotate right with carry RL H)
            clk->add_cycles(8);
            break;
        case 0x1d:  // Rotate contents of L register right and store bit 0 in CF, flags updated
            mem->reg_rr(RL, false); // Carry = false (Rotate right with carry RL L)
            clk->add_cycles(8);
            break;
        case 0x1e:  // Rotate contents of byte at (HL) right and store bit 0 in CF, flags updated
            mem->rr_from_pointer(RL, false); // Carry = false (Rotate right with carry RL L)
            clk->add_cycles(16);
            break;
        case 0x27:  // Shift contents of register A left and store bit 7 in CF, bit0 = 0, flags updated
            mem->reg_sla(RA);
            clk->add_cycles(8);
            break;
        case 0x20:  // Shift contents of register B left and store bit 7 in CF, bit0 = 0, flags updated
            mem->reg_sla(RB);
            clk->add_cycles(8);
            break;
        case 0x21:  // Shift contents of register C left and store bit 7 in CF, bit0 = 0, flags updated
            mem->reg_sla(RC);
            clk->add_cycles(8);
            break;
        case 0x22:  // Shift contents of register D left and store bit 7 in CF, bit0 = 0, flags updated
            mem->reg_sla(RD);
            clk->add_cycles(8);
            break;
        case 0x23:  // Shift contents of register E left and store bit 7 in CF, bit0 = 0, flags updated
            mem->reg_sla(RE);
            clk->add_cycles(8);
            break;
        case 0x24:  // Shift contents of register H left and store bit 7 in CF, bit0 = 0, flags updated
            mem->reg_sla(RH);
            clk->add_cycles(8);
            break;
        case 0x25:  // Shift contents of register L left and store bit 7 in CF, bit0 = 0, flags updated
            mem->reg_sla(RL);
            clk->add_cycles(8);
            break;
        case 0x26:  // Shift contents of byte at (HL) left and store bit 7 in CF, bit0 = 0, flags updated
            mem->sla_from_pointer(RHL);
            clk->add_cycles(16);
            break;
        case 0x2f:  // Shift contents of register A right and store bit 0 in CF, bit7 unchanged, flags updated
            mem->reg_sra(RA);
            clk->add_cycles(8);
            break;
        case 0x28:  // Shift contents of register B right and store bit 0 in CF, bit7 unchanged, flags updated
            mem->reg_sra(RB);
            clk->add_cycles(8);
            break;
        case 0x29:  // Shift contents of register C right and store bit 0 in CF, bit7 unchanged, flags updated
            mem->reg_sra(RC);
            clk->add_cycles(8);
            break;
        case 0x2a:  // Shift contents of register D right and store bit 0 in CF, bit7 unchanged, flags updated
            mem->reg_sra(RD);
            clk->add_cycles(8);
            break;
        case 0x2b:  // Shift contents of register E right and store bit 0 in CF, bit7 unchanged, flags updated
            mem->reg_sra(RE);
            clk->add_cycles(8);
            break;
        case 0x2c:  // Shift contents of register H right and store bit 0 in CF, bit7 unchanged, flags updated
            mem->reg_sra(RH);
            clk->add_cycles(8);
            break;
        case 0x2d:  // Shift contents of register L right and store bit 0 in CF, bit7 unchanged, flags updated
            mem->reg_sra(RL);
            clk->add_cycles(8);
            break;
        case 0x2e:  // Shift contents byte at (HL) right and store bit 0 in CF, bit7 unchanged, flags updated
            mem->sra_from_pointer(RHL);
            clk->add_cycles(16);
            break;
        case 0x3f:  // Shift contents of register A right and store bit 0 in CF, bit7 = 0, flags updated
            mem->reg_srl(RA);
            clk->add_cycles(8);
            break;
        case 0x38:  // Shift contents of register B right and store bit 0 in CF, bit7 = 0, flags updated
            mem->reg_srl(RB);
            clk->add_cycles(8);
            break;
        case 0x39:  // Shift contents of register C right and store bit 0 in CF, bit7 = 0, flags updated
            mem->reg_srl(RC);
            clk->add_cycles(8);
            break;
        case 0x3a:  // Shift contents of register D right and store bit 0 in CF, bit7 = 0, flags updated
            mem->reg_srl(RD);
            clk->add_cycles(8);
            break;
        case 0x3b:  // Shift contents of register E right and store bit 0 in CF, bit7 = 0, flags updated
            mem->reg_srl(RE);
            clk->add_cycles(8);
            break;
        case 0x3c:  // Shift contents of register H right and store bit 0 in CF, bit7 = 0, flags updated
            mem->reg_srl(RH);
            clk->add_cycles(8);
            break;
        case 0x3d:  // Shift contents of register L right and store bit 0 in CF, bit7 = 0, flags updated
            mem->reg_srl(RL);
            clk->add_cycles(8);
            break;
        case 0x3e:  // Shift contents byte at (HL) right and store bit 0 in CF, bit7 = 0, flags updated
            mem->srl_from_pointer(RHL);
            clk->add_cycles(16);
            break;
        case 0x47:  // Test bit 0 in register A and set flags accordingly
            mem->bit_test(RA, 0);
            clk->add_cycles(8);
            break;
        case 0x40:  // Test bit 0 in register B and set flags accordingly
            mem->bit_test(RB, 0);
            clk->add_cycles(8);
            break;
        case 0x41:  // Test bit 0 in register C and set flags accordingly
            mem->bit_test(RC, 0);
            clk->add_cycles(8);
            break;
        case 0x42:  // Test bit 0 in register D and set flags accordingly
            mem->bit_test(RD, 0);
            clk->add_cycles(8);
            break;
        case 0x43:  // Test bit 0 in register E and set flags accordingly
            mem->bit_test(RE, 0);
            clk->add_cycles(8);
            break;
        case 0x44:  // Test bit 0 in register H and set flags accordingly
            mem->bit_test(RH, 0);
            clk->add_cycles(8);
            break;
        case 0x45:  // Test bit 0 in register L and set flags accordingly
            mem->bit_test(RL, 0);
            clk->add_cycles(8);
            break;
        case 0x46:  // Test bit 0 in byte at (HL) and set flags accordingly
            mem->bit_test_from_pointer(RHL, 0);
            clk->add_cycles(16);
            break;
        case 0x4f:  // Test bit 1 in register A and set flags accordingly
            mem->bit_test(RA, 1);
            clk->add_cycles(8);
            break;
        case 0x48:  // Test bit 1 in register B and set flags accordingly
            mem->bit_test(RB, 1);
            clk->add_cycles(8);
            break;
        case 0x49:  // Test bit 1 in register C and set flags accordingly
            mem->bit_test(RC, 1);
            clk->add_cycles(8);
            break;
        case 0x4a:  // Test bit 1 in register D and set flags accordingly
            mem->bit_test(RD, 1);
            clk->add_cycles(8);
            break;
        case 0x4b:  // Test bit 1 in register E and set flags accordingly
            mem->bit_test(RE, 1);
            clk->add_cycles(8);
            break;
        case 0x4c:  // Test bit 1 in register H and set flags accordingly
            mem->bit_test(RH, 1);
            clk->add_cycles(8);
            break;
        case 0x4d:  // Test bit 1 in register L and set flags accordingly
            mem->bit_test(RL, 1);
            clk->add_cycles(8);
            break;
        case 0x4e:  // Test bit 1 in byte at (HL) and set flags accordingly
            mem->bit_test_from_pointer(RHL, 1);
            clk->add_cycles(16);
            break;
        case 0x57:  // Test bit 2 in register A and set flags accordingly
            mem->bit_test(RA, 2);
            clk->add_cycles(8);
            break;
        case 0x50:  // Test bit 2 in register B and set flags accordingly
            mem->bit_test(RB, 2);
            clk->add_cycles(8);
            break;
        case 0x51:  // Test bit 2 in register C and set flags accordingly
            mem->bit_test(RC, 2);
            clk->add_cycles(8);
            break;
        case 0x52:  // Test bit 2 in register D and set flags accordingly
            mem->bit_test(RD, 2);
            clk->add_cycles(8);
            break;
        case 0x53:  // Test bit 2 in register E and set flags accordingly
            mem->bit_test(RE, 2);
            clk->add_cycles(8);
            break;
        case 0x54:  // Test bit 2 in register H and set flags accordingly
            mem->bit_test(RH, 2);
            clk->add_cycles(8);
            break;
        case 0x55:  // Test bit 2 in register L and set flags accordingly
            mem->bit_test(RL, 2);
            clk->add_cycles(8);
            break;
        case 0x56:  // Test bit 2 in byte at (HL) and set flags accordingly
            mem->bit_test_from_pointer(RHL, 2);
            clk->add_cycles(16);
            break;
        case 0x5f:  // Test bit 3 in register A and set flags accordingly
            mem->bit_test(RA, 3);
            clk->add_cycles(8);
            break;
        case 0x58:  // Test bit 3 in register B and set flags accordingly
            mem->bit_test(RB, 3);
            clk->add_cycles(8);
            break;
        case 0x59:  // Test bit 3 in register C and set flags accordingly
            mem->bit_test(RC, 3);
            clk->add_cycles(8);
            break;
        case 0x5a:  // Test bit 3 in register D and set flags accordingly
            mem->bit_test(RE, 3);
            clk->add_cycles(8);
            break;
        case 0x5b:  // Test bit 3 in register E and set flags accordingly
            mem->bit_test(RD, 3);
            clk->add_cycles(8);
            break;
        case 0x5c:  // Test bit 3 in register H and set flags accordingly
            mem->bit_test(RH, 3);
            clk->add_cycles(8);
            break;
        case 0x5d:  // Test bit 3 in register L and set flags accordingly
            mem->bit_test(RL, 3);
            clk->add_cycles(8);
            break;
        case 0x5e:  // Test bit 3 in byte at (HL) and set flags accordingly
            mem->bit_test_from_pointer(RHL, 3);
            clk->add_cycles(16);
            break;
        case 0x67:  // Test bit 4 in register A and set flags accordingly
            mem->bit_test(RA, 4);
            clk->add_cycles(8);
            break;
        case 0x60:  // Test bit 4 in register B and set flags accordingly
            mem->bit_test(RB, 4);
            clk->add_cycles(8);
            break;
        case 0x61:  // Test bit 4 in register C and set flags accordingly
            mem->bit_test(RC, 4);
            clk->add_cycles(8);
            break;
        case 0x62:  // Test bit 4 in register D and set flags accordingly
            mem->bit_test(RD, 4);
            clk->add_cycles(8);
            break;
        case 0x63:  // Test bit 4 in register E and set flags accordingly
            mem->bit_test(RE, 4);
            clk->add_cycles(8);
            break;
        case 0x64:  // Test bit 4 in register H and set flags accordingly
            mem->bit_test(RH, 4);
            clk->add_cycles(8);
            break;
        case 0x65:  // Test bit 4 in register L and set flags accordingly
            mem->bit_test(RL, 4);
            clk->add_cycles(8);
            break;
        case 0x66:  // Test bit 4 in byte at (HL) and set flags accordingly
            mem->bit_test_from_pointer(RHL, 4);
            clk->add_cycles(16);
            break;
        case 0x6f:  // Test bit 5 in register A and set flags accordingly
            mem->bit_test(RA, 5);
            clk->add_cycles(8);
            break;
        case 0x68:  // Test bit 5 in register B and set flags accordingly
            mem->bit_test(RB, 5);
            clk->add_cycles(8);
            break;
        case 0x69:  // Test bit 5 in register C and set flags accordingly
            mem->bit_test(RC, 5);
            clk->add_cycles(8);
            break;
        case 0x6a:  // Test bit 5 in register D and set flags accordingly
            mem->bit_test(RD, 5);
            clk->add_cycles(8);
            break;
        case 0x6b:  // Test bit 5 in register E and set flags accordingly
            mem->bit_test(RE, 5);
            clk->add_cycles(8);
            break;
        case 0x6c:  // Test bit 5 in register H and set flags accordingly
            mem->bit_test(RH, 5);
            clk->add_cycles(8);
            break;
        case 0x6d:  // Test bit 5 in register L and set flags accordingly
            mem->bit_test(RL, 5);
            clk->add_cycles(8);
            break;
        case 0x6e:  // Test bit 5 in byte at (HL) and set flags accordingly
            mem->bit_test_from_pointer(RHL, 5);
            clk->add_cycles(16);
            break;
        case 0x77:  // Test bit 6 in register A and set flags accordingly
            mem->bit_test(RA, 6);
            clk->add_cycles(8);
            break;
        case 0x70:  // Test bit 6 in register B and set flags accordingly
            mem->bit_test(RB, 6);
            clk->add_cycles(8);
            break;
        case 0x71:  // Test bit 6 in register C and set flags accordingly
            mem->bit_test(RC, 6);
            clk->add_cycles(8);
            break;
        case 0x72:  // Test bit 6 in register D and set flags accordingly
            mem->bit_test(RD, 6);
            clk->add_cycles(8);
            break;
        case 0x73:  // Test bit 6 in register E and set flags accordingly
            mem->bit_test(RE, 6);
            clk->add_cycles(8);
            break;
        case 0x74:  // Test bit 6 in register H and set flags accordingly
            mem->bit_test(RH, 6);
            clk->add_cycles(8);
            break;
        case 0x75:  // Test bit 6 in register L and set flags accordingly
            mem->bit_test(RL, 6);
            clk->add_cycles(8);
            break;
        case 0x76:  // Test bit 6 in byte at (HL) and set flags accordingly
            mem->bit_test_from_pointer(RHL, 6);
            clk->add_cycles(16);
            break;
        case 0x7f:  // Test bit 7 in register A and set flags accordingly
            mem->bit_test(RA, 7);
            clk->add_cycles(8);
            break;
        case 0x78:  // Test bit 7 in register B and set flags accordingly
            mem->bit_test(RB, 7);
            clk->add_cycles(8);
            break;
        case 0x79:  // Test bit 7 in register C and set flags accordingly
            mem->bit_test(RC, 7);
            clk->add_cycles(8);
            break;
        case 0x7a:  // Test bit 7 in register D and set flags accordingly
            mem->bit_test(RD, 7);
            clk->add_cycles(8);
            break;
        case 0x7b:  // Test bit 7 in register E and set flags accordingly
            mem->bit_test(RE, 7);
            clk->add_cycles(8);
            break;
        case 0x7c:  // Test bit 7 in register H and set flags accordingly
            mem->bit_test(RH, 7);
            clk->add_cycles(8);
            break;
        case 0x7d:  // Test bit 7 in register L and set flags accordingly
            mem->bit_test(RL, 7);
            clk->add_cycles(8);
            break;
        case 0x7e:  // Test bit 7 in byte at (HL) and set flags accordingly
            mem->bit_test_from_pointer(RHL, 7);
            clk->add_cycles(16);
            break;
        case 0xc7:  // Set bit 0 in register A, flags not affected
            mem->bit_set(RA, 0);
            clk->add_cycles(8);
            break;
        case 0xc0:  // Set bit 0 in register B, flags not affected
            mem->bit_set(RB, 0);
            clk->add_cycles(8);
            break;
        case 0xc1:  // Set bit 0 in register C, flags not affected
            mem->bit_set(RC, 0);
            clk->add_cycles(8);
            break;
        case 0xc2:  // Set bit 0 in register D, flags not affectedy
            mem->bit_set(RD, 0);
            clk->add_cycles(8);
            break;
        case 0xc3:  // Set bit 0 in register E, flags not affected
            mem->bit_set(RE, 0);
            clk->add_cycles(8);
            break;
        case 0xc4:  // Set bit 0 in register H, flags not affected
            mem->bit_set(RH, 0);
            clk->add_cycles(8);
            break;
        case 0xc5:  // Set bit 0 in register L, flags not affected
            mem->bit_set(RL, 0);
            clk->add_cycles(8);
            break;
        case 0xc6:  // Set bit 0 in byte at (HL), flags not affected
            mem->bit_set_from_pointer(RHL, 0);
            clk->add_cycles(16);
            break;
        case 0xcf:  // Set bit 1 in register A, flags not affected
            mem->bit_set(RA, 1);
            clk->add_cycles(8);
            break;
        case 0xc8:  // Set bit 1 in register B, flags not affected
            mem->bit_set(RB, 1);
            clk->add_cycles(8);
            break;
        case 0xc9:  // Set bit 1 in register C, flags not affected
            mem->bit_set(RC, 1);
            clk->add_cycles(8);
            break;
        case 0xca:  // Set bit 1 in register D, flags not affectedy
            mem->bit_set(RD, 1);
            clk->add_cycles(8);
            break;
        case 0xcb:  // Set bit 1 in register E, flags not affected
            mem->bit_set(RE, 1);
            clk->add_cycles(8);
            break;
        case 0xcc:  // Set bit 1 in register H, flags not affected
            mem->bit_set(RH, 1);
            clk->add_cycles(8);
            break;
        case 0xcd:  // Set bit 1 in register L, flags not affected
            mem->bit_set(RL, 1);
            clk->add_cycles(8);
            break;
        case 0xce:  // Set bit 1 in byte at (HL), flags not affected
            mem->bit_set_from_pointer(RHL, 1);
            clk->add_cycles(16);
            break;
        case 0xd7:  // Set bit 2 in register A, flags not affected
            mem->bit_set(RA, 2);
            clk->add_cycles(8);
            break;
        case 0xd0:  // Set bit 2 in register B, flags not affected
            mem->bit_set(RB, 2);
            clk->add_cycles(8);
            break;
        case 0xd1:  // Set bit 2 in register C, flags not affected
            mem->bit_set(RC, 2);
            clk->add_cycles(8);
            break;
        case 0xd2:  // Set bit 2 in register D, flags not affectedy
            mem->bit_set(RD, 2);
            clk->add_cycles(8);
            break;
        case 0xd3:  // Set bit 2 in register E, flags not affected
            mem->bit_set(RE, 2);
            clk->add_cycles(8);
            break;
        case 0xd4:  // Set bit 2 in register H, flags not affected
            mem->bit_set(RH, 2);
            clk->add_cycles(8);
            break;
        case 0xd5:  // Set bit 2 in register L, flags not affected
            mem->bit_set(RL, 2);
            clk->add_cycles(8);
            break;
        case 0xd6:  // Set bit 2 in byte at (HL), flags not affected
            mem->bit_set_from_pointer(RHL, 2);
            clk->add_cycles(16);
            break;
        case 0xdf:  // Set bit 3 in register A, flags not affected
            mem->bit_set(RA, 3);
            clk->add_cycles(8);
            break;
        case 0xd8:  // Set bit 3 in register B, flags not affected
            mem->bit_set(RB, 3);
            clk->add_cycles(8);
            break;
        case 0xd9:  // Set bit 3 in register C, flags not affected
            mem->bit_set(RC, 3);
            clk->add_cycles(8);
            break;
        case 0xda:  // Set bit 3 in register D, flags not affectedy
            mem->bit_set(RD, 3);
            clk->add_cycles(8);
            break;
        case 0xdb:  // Set bit 3 in register E, flags not affected
            mem->bit_set(RE, 3);
            clk->add_cycles(8);
            break;
        case 0xdc:  // Set bit 3 in register H, flags not affected
            mem->bit_set(RH, 3);
            clk->add_cycles(8);
            break;
        case 0xdd:  // Set bit 3 in register L, flags not affected
            mem->bit_set(RL, 3);
            clk->add_cycles(8);
            break;
        case 0xde:  // Set bit 3 in byte at (HL), flags not affected
            mem->bit_set_from_pointer(RHL, 3);
            clk->add_cycles(16);
            break;
        case 0xe7:  // Set bit 4 in register A, flags not affected
            mem->bit_set(RA, 4);
            clk->add_cycles(8);
            break;
        case 0xe0:  // Set bit 4 in register B, flags not affected
            mem->bit_set(RB, 4);
            clk->add_cycles(8);
            break;
        case 0xe1:  // Set bit 4 in register C, flags not affected
            mem->bit_set(RC, 4);
            clk->add_cycles(8);
            break;
        case 0xe2:  // Set bit 4 in register D, flags not affectedy
            mem->bit_set(RD, 4);
            clk->add_cycles(8);
            break;
        case 0xe3:  // Set bit 4 in register E, flags not affected
            mem->bit_set(RE, 4);
            clk->add_cycles(8);
            break;
        case 0xe4:  // Set bit 4 in register H, flags not affected
            mem->bit_set(RH, 4);
            clk->add_cycles(8);
            break;
        case 0xe5:  // Set bit 4 in register L, flags not affected
            mem->bit_set(RL, 4);
            clk->add_cycles(8);
            break;
        case 0xe6:  // Set bit 4 in byte at (HL), flags not affected
            mem->bit_set_from_pointer(RHL, 4);
            clk->add_cycles(16);
            break;
        case 0xef:  // Set bit 5 in register A, flags not affected
            mem->bit_set(RA, 5);
            clk->add_cycles(8);
            break;
        case 0xe8:  // Set bit 5 in register B, flags not affected
            mem->bit_set(RB, 5);
            clk->add_cycles(8);
            break;
        case 0xe9:  // Set bit 5 in register C, flags not affected
            mem->bit_set(RC, 5);
            clk->add_cycles(8);
            break;
        case 0xea:  // Set bit 5 in register D, flags not affectedy
            mem->bit_set(RD, 5);
            clk->add_cycles(8);
            break;
        case 0xeb:  // Set bit 5 in register E, flags not affected
            mem->bit_set(RE, 5);
            clk->add_cycles(8);
            break;
        case 0xec:  // Set bit 5 in register H, flags not affected
            mem->bit_set(RH, 5);
            clk->add_cycles(8);
            break;
        case 0xed:  // Set bit 5 in register L, flags not affected
            mem->bit_set(RL, 5);
            clk->add_cycles(8);
            break;
        case 0xee:  // Set bit 5 in byte at (HL), flags not affected
            mem->bit_set_from_pointer(RHL, 5);
            clk->add_cycles(16);
            break;
       case 0xf7:  // Set bit 6 in register A, flags not affected
            mem->bit_set(RA, 6);
            clk->add_cycles(8);
            break;
        case 0xf0:  // Set bit 6 in register B, flags not affected
            mem->bit_set(RB, 6);
            clk->add_cycles(8);
            break;
        case 0xf1:  // Set bit 6 in register C, flags not affected
            mem->bit_set(RC, 6);
            clk->add_cycles(8);
            break;
        case 0xf2:  // Set bit 6 in register D, flags not affectedy
            mem->bit_set(RD, 6);
            clk->add_cycles(8);
            break;
        case 0xf3:  // Set bit 6 in register E, flags not affected
            mem->bit_set(RE, 6);
            clk->add_cycles(8);
            break;
        case 0xf4:  // Set bit 6 in register H, flags not affected
            mem->bit_set(RH, 6);
            clk->add_cycles(8);
            break;
        case 0xf5:  // Set bit 6 in register L, flags not affected
            mem->bit_set(RL, 6);
            clk->add_cycles(8);
            break;
        case 0xf6:  // Set bit 6 in byte at (HL), flags not affected
            mem->bit_set_from_pointer(RHL, 6);
            clk->add_cycles(16);
            break;
        case 0xff:  // Set bit 7 in register A, flags not affected
            mem->bit_set(RA, 7);
            clk->add_cycles(8);
            break;
        case 0xf8:  // Set bit 7 in register B, flags not affected
            mem->bit_set(RB, 7);
            clk->add_cycles(8);
            break;
        case 0xf9:  // Set bit 7 in register C, flags not affected
            mem->bit_set(RC, 7);
            clk->add_cycles(8);
            break;
        case 0xfa:  // Set bit 7 in register D, flags not affectedy
            mem->bit_set(RD, 7);
            clk->add_cycles(8);
            break;
        case 0xfb:  // Set bit 7 in register E, flags not affected
            mem->bit_set(RE, 7);
            clk->add_cycles(8);
            break;
        case 0xfc:  // Set bit 7 in register H, flags not affected
            mem->bit_set(RH, 7);
            clk->add_cycles(8);
            break;
        case 0xfd:  // Set bit 7 in register L, flags not affected
            mem->bit_set(RL, 7);
            clk->add_cycles(8);
            break;
        case 0xfe:  // Set bit 7 in byte at (HL), flags not affected
            mem->bit_set_from_pointer(RHL, 7);
            clk->add_cycles(16);
            break;
        case 0x87:  // Reset bit 0 in register A, flags not affected
            mem->bit_res(RA, 0);
            clk->add_cycles(8);
            break;
        case 0x80:  // Reset bit 0 in register B, flags not affected
            mem->bit_res(RB, 0);
            clk->add_cycles(8);
            break;
        case 0x81:  // Reset bit 0 in register C, flags not affected
            mem->bit_res(RC, 0);
            clk->add_cycles(8);
            break;
        case 0x82:  // Reset bit 0 in register D, flags not affectedy
            mem->bit_res(RD, 0);
            clk->add_cycles(8);
            break;
        case 0x83:  // Reset bit 0 in register E, flags not affected
            mem->bit_res(RE, 0);
            clk->add_cycles(8);
            break;
        case 0x84:  // Reset bit 0 in register H, flags not affected
            mem->bit_res(RH, 0);
            clk->add_cycles(8);
            break;
        case 0x85:  // Reset bit 0 in register L, flags not affected
            mem->bit_res(RL, 0);
            clk->add_cycles(8);
            break;
        case 0x86:  // Reset bit 0 in byte at (HL), flags not affected
            mem->bit_res_from_pointer(RHL, 0);
            clk->add_cycles(16);
            break;
        case 0x8f:  // Reset bit 1 in register A, flags not affected
            mem->bit_res(RA, 1);
            clk->add_cycles(8);
            break;
        case 0x88:  // Reset bit 1 in register B, flags not affected
            mem->bit_res(RB, 1);
            clk->add_cycles(8);
            break;
        case 0x89:  // Reset bit 1 in register C, flags not affected
            mem->bit_res(RC, 1);
            clk->add_cycles(8);
            break;
        case 0x8a:  // Reset bit 1 in register D, flags not affectedy
            mem->bit_res(RD, 1);
            clk->add_cycles(8);
            break;
        case 0x8b:  // Reset bit 1 in register E, flags not affected
            mem->bit_res(RE, 1);
            clk->add_cycles(8);
            break;
        case 0x8c:  // Reset bit 1 in register H, flags not affected
            mem->bit_res(RH, 1);
            clk->add_cycles(8);
            break;
        case 0x8d:  // Reset bit 1 in register L, flags not affected
            mem->bit_res(RL, 1);
            clk->add_cycles(8);
            break;
        case 0x8e:  // Reset bit 1 in byte at (HL), flags not affected
            mem->bit_res_from_pointer(RHL, 1);
            clk->add_cycles(16);
            break;
        case 0x97:  // Reset bit 2 in register A, flags not affected
            mem->bit_res(RA, 2);
            clk->add_cycles(8);
            break;
        case 0x90:  // Reset bit 2 in register B, flags not affected
            mem->bit_res(RB, 2);
            clk->add_cycles(8);
            break;
        case 0x91:  // Reset bit 2 in register C, flags not affected
            mem->bit_res(RC, 2);
            clk->add_cycles(8);
            break;
        case 0x92:  // Reset bit 2 in register D, flags not affectedy
            mem->bit_res(RD, 2);
            clk->add_cycles(8);
            break;
        case 0x93:  // Reset bit 2 in register E, flags not affected
            mem->bit_res(RE, 2);
            clk->add_cycles(8);
            break;
        case 0x94:  // Reset bit 2 in register H, flags not affected
            mem->bit_res(RH, 2);
            clk->add_cycles(8);
            break;
        case 0x95:  // Reset bit 2 in register L, flags not affected
            mem->bit_res(RL, 2);
            clk->add_cycles(8);
            break;
        case 0x96:  // Reset bit 2 in byte at (HL), flags not affected
            mem->bit_res_from_pointer(RHL, 2);
            clk->add_cycles(16);
            break;
        case 0x9f:  // Reset bit 3 in register A, flags not affected
            mem->bit_res(RA, 3);
            clk->add_cycles(8);
            break;
        case 0x98:  // Reset bit 3 in register B, flags not affected
            mem->bit_res(RB, 3);
            clk->add_cycles(8);
            break;
        case 0x99:  // Reset bit 3 in register C, flags not affected
            mem->bit_res(RC, 3);
            clk->add_cycles(8);
            break;
        case 0x9a:  // Reset bit 3 in register D, flags not affectedy
            mem->bit_res(RD, 3);
            clk->add_cycles(8);
            break;
        case 0x9b:  // Reset bit 3 in register E, flags not affected
            mem->bit_res(RE, 3);
            clk->add_cycles(8);
            break;
        case 0x9c:  // Reset bit 3 in register H, flags not affected
            mem->bit_res(RH, 3);
            clk->add_cycles(8);
            break;
        case 0x9d:  // Reset bit 3 in register L, flags not affected
            mem->bit_res(RL, 3);
            clk->add_cycles(8);
            break;
        case 0x9e:  // Reset bit 3 in byte at (HL), flags not affected
            mem->bit_res_from_pointer(RHL, 3);
            clk->add_cycles(16);
            break;
        case 0xa7:  // Reset bit 4 in register A, flags not affected
            mem->bit_res(RA, 4);
            clk->add_cycles(8);
            break;
        case 0xa0:  // Reset bit 4 in register B, flags not affected
            mem->bit_res(RB, 4);
            clk->add_cycles(8);
            break;
        case 0xa1:  // Reset bit 4 in register C, flags not affected
            mem->bit_res(RC, 4);
            clk->add_cycles(8);
            break;
        case 0xa2:  // Reset bit 4 in register D, flags not affectedy
            mem->bit_res(RD, 4);
            clk->add_cycles(8);
            break;
        case 0xa3:  // Reset bit 4 in register E, flags not affected
            mem->bit_res(RE, 4);
            clk->add_cycles(8);
            break;
        case 0xa4:  // Reset bit 4 in register H, flags not affected
            mem->bit_res(RH, 4);
            clk->add_cycles(8);
            break;
        case 0xa5:  // Reset bit 4 in register L, flags not affected
            mem->bit_res(RL, 4);
            clk->add_cycles(8);
            break;
        case 0xa6:  // Reset bit 4 in byte at (HL), flags not affected
            mem->bit_res_from_pointer(RHL, 4);
            clk->add_cycles(16);
            break;
        case 0xaf:  // Reset bit 5 in register A, flags not affected
            mem->bit_res(RA, 5);
            clk->add_cycles(8);
            break;
        case 0xa8:  // Reset bit 5 in register B, flags not affected
            mem->bit_res(RB, 5);
            clk->add_cycles(8);
            break;
        case 0xa9:  // Reset bit 5 in register C, flags not affected
            mem->bit_res(RC, 5);
            clk->add_cycles(8);
            break;
        case 0xaa:  // Reset bit 5 in register D, flags not affectedy
            mem->bit_res(RD, 5);
            clk->add_cycles(8);
            break;
        case 0xab:  // Reset bit 5 in register E, flags not affected
            mem->bit_res(RE, 5);
            clk->add_cycles(8);
            break;
        case 0xac:  // Reset bit 5 in register H, flags not affected
            mem->bit_res(RH, 5);
            clk->add_cycles(8);
            break;
        case 0xad:  // Reset bit 5 in register L, flags not affected
            mem->bit_res(RL, 5);
            clk->add_cycles(8);
            break;
        case 0xae:  // Reset bit 5 in byte at (HL), flags not affected
            mem->bit_res_from_pointer(RHL, 5);
            clk->add_cycles(16);
            break;
        case 0xb7:  // Reset bit 6 in register A, flags not affected
            mem->bit_res(RA, 6);
            clk->add_cycles(8);
            break;
        case 0xb0:  // Reset bit 6 in register B, flags not affected
            mem->bit_res(RB, 6);
            clk->add_cycles(8);
            break;
        case 0xb1:  // Reset bit 6 in register C, flags not affected
            mem->bit_res(RC, 6);
            clk->add_cycles(8);
            break;
        case 0xb2:  // Reset bit 6 in register D, flags not affectedy
            mem->bit_res(RD, 6);
            clk->add_cycles(8);
            break;
        case 0xb3:  // Reset bit 6 in register E, flags not affected
            mem->bit_res(RE, 6);
            clk->add_cycles(8);
            break;
        case 0xb4:  // Reset bit 6 in register H, flags not affected
            mem->bit_res(RH, 6);
            clk->add_cycles(8);
            break;
        case 0xb5:  // Reset bit 6 in register L, flags not affected
            mem->bit_res(RL, 6);
            clk->add_cycles(8);
            break;
        case 0xb6:  // Reset bit 6 in byte at (HL), flags not affected
            mem->bit_res_from_pointer(RHL, 6);
            clk->add_cycles(16);
            break;
        case 0xbf:  // Reset bit 7 in register A, flags not affected
            mem->bit_res(RA, 7);
            clk->add_cycles(8);
            break;
        case 0xb8:  // Reset bit 7 in register B, flags not affected
            mem->bit_res(RB, 7);
            clk->add_cycles(8);
            break;
        case 0xb9:  // Reset bit 7 in register C, flags not affected
            mem->bit_res(RC, 7);
            clk->add_cycles(8);
            break;
        case 0xba:  // Reset bit 7 in register D, flags not affectedy
            mem->bit_res(RD, 7);
            clk->add_cycles(8);
            break;
        case 0xbb:  // Reset bit 7 in register E, flags not affected
            mem->bit_res(RE, 7);
            clk->add_cycles(8);
            break;
        case 0xbc:  // Reset bit 7 in register H, flags not affected
            mem->bit_res(RH, 7);
            clk->add_cycles(8);
            break;
        case 0xbd:  // Reset bit 7 in register L, flags not affected
            mem->bit_res(RL, 7);
            clk->add_cycles(8);
            break;
        case 0xbe:  // Reset bit 7 in byte at (HL), flags not affected
            mem->bit_res_from_pointer(RHL, 7);
            clk->add_cycles(16);
            break;
    }
}
