#include "memory.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/// RAM/ROM Functions

// Read byte from RAM/ROM
uint8_t Memory::get_byte(uint16_t address)
{
    return ram[address];
}

// Read a bit from an byte in RAM/ROM
uint8_t Memory::get_bit(uint16_t address, uint8_t bit_number)
{
	uint8_t bit_mask;
	bit_mask = 1 << bit_number;
	return (ram[address] & bit_mask) >> bit_number;
}

// Read byte from ROM and increment PC
uint8_t Memory::fetch_byte()
{
    // Need to return current byte before incrementing PC
    return ram[pc++];
}

// Read byte from RAM/ROM pointed to by 16-bit register
uint8_t Memory::get_from_pointer(uint8_t reg_id)
{
    // Check if valid 16-bit register used
    if ((reg_id > REG_16_START) && (reg_id < REG_16_END))
    {
        return get_byte(reg_get16(reg_id));
    }
    else
    {
        throw "Invalid register id";
        return 0; // invalid register
    }
}

// Set byte at RAM address pointed to by 16-bit register to byte value
void Memory::set_from_pointer(uint8_t reg_id, uint8_t byte_value)
{
    // Check if valid 16-bit register used
    if ((reg_id > REG_16_START) && (reg_id < REG_16_END))
    {
        write_byte(reg_get16(reg_id), byte_value);
    }
    else
    {
        throw "Invalid register id";
    }
}

// Write byte to RAM/ROM
void Memory::write_byte(uint16_t address, uint8_t byte)
{
    ram[address] = byte;

    // Check for DMA Register Write
    if (address == R_DMA)
    {
    	dma_transfer();
    }
}

// Update individual bit in RAM/ROM
void Memory::write_bit(uint16_t address, uint8_t bit_number, uint8_t bit_val)
{
	// Get byte
	uint8_t current_value, new_value, bit_mask;
	current_value = ram[address];

	// Create mask
	bit_mask = 1 << bit_number;

	if (bit_val == 1)
	{
		new_value = current_value | bit_mask;
	}
	else if (bit_val == 0)
	{
		new_value = current_value & ~bit_mask;
	}

	// Update value
	ram[address] = new_value;
}

// Increment byte and update flags
void Memory::inc_from_pointer(uint8_t reg_id)
{
    // Get byte
    uint8_t byte_in;
    byte_in = get_from_pointer(reg_id);

    // Update H flag
    flag_update(HF,(byte_in & 0xf0) == ((byte_in + 1) & 0xf0));

    // Clear N flag
    flag_update(NF,0);

    // Update 8 bit register
    byte_in++;
    set_from_pointer(RHL,byte_in);

    // Update Z flag
    flag_update(ZF,(byte_in==0));
}

// Decrement byte and update flags
void Memory::dec_from_pointer(uint8_t reg_id)
{
    // Get byte
    uint8_t byte_in;
    byte_in = get_from_pointer(reg_id);

    // Update H flag
    flag_update(HF,(byte_in & 0xf0) == ((byte_in - 1) & 0xf0));

    // Clear N flag
    flag_update(NF,0);

    // Update 8 bit register
    byte_in--;
    set_from_pointer(RHL,byte_in);

    // Update Z flag
    flag_update(ZF,(byte_in==0));
}

// Swap upper and lower nibbles of register, flags updated
void Memory::swap_from_pointer(uint8_t reg_id)
{
    // Get the byte
    uint8_t byte_in, new_val;
    byte_in = get_from_pointer(reg_id);

    // Perform the swap
    new_val = (byte_in >> 4) | ((byte_in & 0xf) << 4);

    // Update byte
    set_from_pointer(RHL, new_val);

    // Update H flag
    flag_update(HF,0);

    // Update C flag
    flag_update(CF,0);

    // Update Z flag
    flag_update(ZF,(new_val==0));

    // Clear N flag
    flag_update(NF,0);
}

// Rotate contents of byte at (n) left and store bit 7 in CF, flags updated
// When carry is true, bit 0 = previous bit 7
// When carry is false, bit 0 = previous CF
// Flags Z 0 0 C
void Memory::rl_from_pointer(uint8_t reg_id, bool carry)
{
    uint8_t byte_in, a_value, bit7;

    // Get byte
    byte_in = get_from_pointer(reg_id);

    // Get value of bit 7
    bit7 = (byte_in & 0x80) >> 7;

    // Shift register left by 1
    a_value = byte_in << 1;

    if (carry)
    {
        // Append previous bit 7 to bit 0
        a_value += bit7;
        set_from_pointer(reg_id, a_value);
    }
    else
    {
        // Append previous CF to bit 0
        a_value += flag_get(CF);
        set_from_pointer(reg_id, a_value);
    }

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(a_value==0));

    // Set CF to bit7
    flag_update(CF,(bool)bit7);
}

// Rotate contents of byte at (n) right and store bit 0 in CF, flags updated
// When carry is true, bit 7 = previous bit 0
// When carry is false, bit 7 = previous CF
// Flags Z 0 0 C
void Memory::rr_from_pointer(uint8_t reg_id, bool carry)
{
    uint8_t byte_in, a_value, bit0;

    // Get byte
    byte_in = get_from_pointer(reg_id);

    // Get value of bit 0
    bit0 = byte_in & 0x01;

    // Shift register right by 1
    a_value = byte_in >> 1;

    if (carry)
    {
        // Append previous bit 0 to bit 7
        a_value += (bit0 << 7);
        set_from_pointer(reg_id, a_value);
    }
    else
    {
        // Append previous CF to bit 7
        a_value += (flag_get(CF) << 7);
        set_from_pointer(reg_id, a_value);
    }

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(a_value==0));

    // Set CF to bit0
    flag_update(CF,(bool)bit0);
}

// Shift contents of byte at (n) left and store bit 7 in CF, bit0 = 0, flags updated
// Flags Z 0 0 C
void Memory::sla_from_pointer(uint8_t reg_id)
{
    uint8_t byte_in, byte_val, bit7;

    // Get byte
    byte_in = get_from_pointer(reg_id);

    // Get value of bit 7
    bit7 = (byte_in & 0x80) >> 7;

    // Shift register left by 1
    byte_val = byte_in << 1;
    set_from_pointer(reg_id, byte_val);

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(byte_val==0));

    // Set CF to bit7
    flag_update(CF,(bool)bit7);
}

// Shift contents of byte at (n) right and store bit 0 in CF, bit7 unchanged, flags updated
// Flags Z 0 0 C
void Memory::sra_from_pointer(uint8_t reg_id)
{
    uint8_t byte_in, byte_val, bit0, bit7;

    // Get byte
    byte_in = get_from_pointer(reg_id);

    // Get value of bit 0
    bit0 = byte_in & 0x01;

    // Get value of bit 7
    bit7 = byte_in & 0x80;

    // Shift register right by 1 and reinstate bit 7
    byte_val = (byte_in >> 1) + bit7;
    set_from_pointer(reg_id, byte_val);

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(byte_val==0));

    // Set CF to bit0
    flag_update(CF,(bool)bit0);
}

// Shift contents of byte at (n) right and store bit 0 in CF, bit7 = 0, flags updated
// Flags Z 0 0 C
void Memory::srl_from_pointer(uint8_t reg_id)
{
    uint8_t byte_in, byte_val, bit0;

    // Get byte
    byte_in = get_from_pointer(reg_id);

    // Get value of bit 0
    bit0 = byte_in & 0x01;

    // Shift register right by 1
    byte_val = byte_in >> 1;
    set_from_pointer(reg_id, byte_val);

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(byte_val==0));

    // Set CF to bit0
    flag_update(CF,(bool)bit0);
}

// Test bit b in byte at (n) and set flags accordingly
// Flags Z 0 1 -
void Memory::bit_test_from_pointer(uint8_t reg_id, uint8_t bit_number)
{
    uint8_t bit_val;

    // Get bit value
    bit_val = get_from_pointer(reg_id) >> bit_number;

    // Update Z flag
    flag_update(ZF,(bit_val==0));

    // Clear N flag
    flag_update(NF,0);

    // Update H flag
    flag_update(HF,1);
}

// Set bit in in byte at (n), flags not affected
void Memory::bit_set_from_pointer(uint8_t reg_id, uint8_t bit_number)
{
    uint8_t byte_val, new_val, bit_mask;

    // Get current byte value
    byte_val = get_from_pointer(reg_id);

    // Set the bit
    bit_mask = 1 << bit_number;
    new_val = byte_val | bit_mask;
    set_from_pointer(reg_id, new_val);
}

// Reset bit in in byte at (n), flags not affected
void Memory::bit_res_from_pointer(uint8_t reg_id, uint8_t bit_number)
{
    uint8_t byte_val, new_val, bit_mask;

    // Get current byte value
    byte_val = get_from_pointer(reg_id);

    // Reset the bit
    bit_mask = 1 << bit_number;
    new_val = byte_val & ~bit_mask;
    set_from_pointer(reg_id, new_val);
}

// Perform DMA transfer from ROM/RAM to OAM
void Memory::dma_transfer()
{
	// Get start address for DMA transfer
	uint16_t dma_start;
	dma_start = ram[R_DMA] << 8;

	//uint32_t data;

	// Transfer data
	for (uint8_t i = 0; i < 0xA0; i++)
	{
		ram[A_OAM + i] = ram[dma_start + i];
	}
}




