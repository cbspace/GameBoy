#include "memory.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/// Logical Functions

// Register A is ANDed with and_value, result stored in register A - flags updated
void Memory::reg_and(uint8_t and_value)
{
    // Update register A
    reg[RA] &= and_value;

    // Update H flag
    flag_update(HF,1);

    // Update C flag
    flag_update(CF,0);

    // Update Z flag
    flag_update(ZF,(reg[RA]==0));

    // Clear N flag
    flag_update(NF,0);
}

// Register A is ORed with and_value, result stored in register A - flags updated
void Memory::reg_or(uint8_t or_value)
{
    // Update register A
    reg[RA] |= or_value;

    // Update H flag
    flag_update(HF,0);

    // Update C flag
    flag_update(CF,0);

    // Update Z flag
    flag_update(ZF,(reg[RA]==0));

    // Clear N flag
    flag_update(NF,0);
}

// Register A is XORed with and_value, result stored in register A - flags updated
void Memory::reg_xor(uint8_t xor_value)
{
    // Update register A
    reg[RA] ^= xor_value;

    // Update H flag
    flag_update(HF,0);

    // Update C flag
    flag_update(CF,0);

    // Update Z flag
    flag_update(ZF,(reg[RA]==0));

    // Clear N flag
    flag_update(NF,0);
}

// Register A is compared with cmp_value, result stored in register A - flags updated
void Memory::reg_compare(uint8_t cmp_value)
{
    int16_t total;

    // The total result
    total = reg[RA] - cmp_value;

    // Update H flag
    flag_update(HF,(reg[RA] & 0xf0) == (total & 0xf0));

    // Update C flag
    flag_update(CF,!(total < 0));

    // Update Z flag
    flag_update(ZF,(total==0));

    // Set N flag
    flag_update(NF,1);
}

// Swap upper and lower nibbles of register, flags updated
void Memory::reg_swap(uint8_t reg_id)
{
    // Update register
    reg[reg_id] = (reg[reg_id] >> 4) | ((reg[reg_id] & 0xf) << 4);

    // Update H flag
    flag_update(HF,0);

    // Update C flag
    flag_update(CF,0);

    // Update Z flag
    flag_update(ZF,(reg[reg_id]==0));

    // Clear N flag
    flag_update(NF,0);
}

// Test bit b in register and set flags accordingly
// Flags Z 0 1 -
void Memory::bit_test(uint8_t reg_id, uint8_t bit_number)
{
    uint8_t bit_val;

    // Get bit value
    bit_val = (reg[reg_id] >> bit_number) & 1;

    // Update Z flag
    flag_update(ZF,(bit_val==0));

    // Clear N flag
    flag_update(NF,0);

    // Update H flag
    flag_update(HF,1);
}

// Set bit in register, flags not affected
void Memory::bit_set(uint8_t reg_id, uint8_t bit_number)
{
    uint8_t new_val, bit_mask;

    // Set the bit
    bit_mask = 1 << bit_number;
    new_val = reg[reg_id] | bit_mask;
    reg[reg_id] = new_val;
}

// Reset bit in register, flags not affected
void Memory::bit_res(uint8_t reg_id, uint8_t bit_number)
{
    uint8_t new_val, bit_mask;

    // Set the bit
    bit_mask = 1 << bit_number;
    new_val = reg[reg_id] & ~bit_mask;
    reg[reg_id] = new_val;
}

