#include "memory.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/// Register Functions

// Get contents of 8 bit register
uint8_t Memory::reg_get(uint8_t reg_id)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        return reg[reg_id];
    }
    else
    {
        throw "Invalid register id";
        return 0; // invalid register
    }
}

// Get contents of 16 bit register
uint16_t Memory::reg_get16(uint8_t reg_id)
{
    switch(reg_id)
    {
        case RAF:   // AF register
            return (reg[RA] << 8) + reg[RF];
        case RBC:   // BC register
            return (reg[RB] << 8) + reg[RC];
        case RDE:   // DE register
            return (reg[RD] << 8) + reg[RE];
        case RHL:   // HL register
            return (reg[RH] << 8) + reg[RL];
        default:    // Invalid register
            throw "Invalid register id";
            return 0;
    }
}

// Set value in 8 bit register
void Memory::reg_set(uint8_t reg_id, uint8_t reg_value)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        reg[reg_id] = reg_value;
    }
    else
    {
        throw "Invalid register id";
    }
    ///temp
    //printf("register %i set to %.2X\n", reg_id, reg_value); // Debug message
}

// Set value in 16 bit register
void Memory::reg_set(uint8_t reg_id, uint16_t reg_value)
{
    uint8_t high_byte;
    uint8_t low_byte;

    high_byte = reg_value >> 8;
    low_byte = reg_value & 0xff;
///temp
//printf("register %i set to %.2X\n", reg_id, reg_value); // Debug message
    switch(reg_id)
    {
        case RAF:   // AF register
            reg[RA] = high_byte;
            reg[RF] = low_byte;
            break;
        case RBC:   // BC register
            reg[RB] = high_byte;
            reg[RC] = low_byte;
            break;
        case RDE:   // DE register
            reg[RD] = high_byte;
            reg[RE] = low_byte;
            break;
        case RHL:   // HL register
            reg[RH] = high_byte;
            reg[RL] = low_byte;
            break;
        default:    // Invalid register
            throw "Invalid register id";
    }
}

// Copy data between 8 bit registers
void Memory::reg_copy(uint8_t to_reg_id, uint8_t from_reg_id)
{
    // Check if registers are valid
    if ((from_reg_id < REG_ARRAY_SIZE) && (to_reg_id < REG_ARRAY_SIZE))
    {
        // use reg_get and reg_set to copy data
        reg_set(to_reg_id, reg_get(from_reg_id));
    }
    else
    {
        throw "Invalid register id";
    }
}

// Increment 8-bit register, flags updated
void Memory::reg_inc(uint8_t reg_id)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        // Update H flag
        flag_update(HF,(reg[reg_id] & 0xf0) == ((reg[reg_id] + 1) & 0xf0));

        // Clear N flag
        flag_update(NF,0);

        // Update 8 bit register
        reg[reg_id]++;

        // Update Z flag
        flag_update(ZF,(reg[reg_id]==0));
    }
    else
    {
        throw "Invalid register id";
    }
}

// Increment 16-bit register, flags not updated
void Memory::reg_inc16(uint8_t reg_id)
{
    uint16_t reg_val;

    // Check if valid 16-bit register used
    if ((reg_id > REG_16_START) && (reg_id < REG_16_END))
    {
        reg_val = reg_get16(reg_id);
        reg_val++;
        reg_set(reg_id,reg_val);
    }
    else
    {
        throw "Invalid register id";
    }
}

// Decrement 16-bit register, flags not updated
void Memory::reg_dec16(uint8_t reg_id)
{
    uint16_t reg_val;

    // Check if valid 16-bit register used
    if ((reg_id > REG_16_START) && (reg_id < REG_16_END))
    {
        reg_val = reg_get16(reg_id);
        reg_val--;
        reg_set(reg_id,reg_val);
    }
    else
    {
        throw "Invalid register id";
    }
}

// Decrement register, flags updated
void Memory::reg_dec(uint8_t reg_id)
{
    // 16 bit register value
    //uint16_t reg_val;

    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        // Update H flag
        flag_update(HF,(reg[reg_id] & 0xf0) == ((reg[reg_id] - 1) & 0xf0));

        // Clear N flag
        flag_update(NF,0);

        // Update 8 bit register
        reg[reg_id]--;

        // Update Z flag
        flag_update(ZF,(reg[reg_id]==0));

        ///temp
        //printf("reg %i now equals %i ", reg_id, reg[reg_id]);
    }
    else
    {
        throw "Invalid register id";
    }
}
