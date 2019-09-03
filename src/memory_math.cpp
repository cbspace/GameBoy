#include "memory.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/// Math Functions

// Add value to 8-bit register and update flags, bool parameter is add with carry
void Memory::reg_add(uint8_t reg_id, uint8_t add_value, bool carry)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        uint16_t add_w_carry, total;

        // Carry is added to accomodate ADDC operations
        add_w_carry = add_value;
        if (carry)
        {
            add_w_carry += flag_get(CF);
        }

        // The total sum
        total = reg[reg_id] + add_w_carry;

        // Update H flag
        flag_update(HF,(reg[reg_id] & 0xf0) == (total & 0xf0));

        // Update C flag
        flag_update(CF,(total > 0xff));

        // Clear N flag
        flag_update(NF,0);

        // Update 8 bit register
        reg[reg_id] += add_value + carry;

        // Update Z flag
        flag_update(ZF,(reg[reg_id]==0));
    }
    else
    {
        throw "Invalid register id";
    }
}

// Subtract value from 8-bit register and flags updated, bool brrow used for sub with borrow
void Memory::reg_sub(uint8_t reg_id, uint8_t sub_value, bool borrow)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        int16_t sub_w_borrow, total;

        // Borrow is subtracted to accomodate SBC operations
        sub_w_borrow = sub_value;
        if (borrow)
        {
            sub_w_borrow -= flag_get(CF);
        }

        // The total result
        total = sub_value - sub_w_borrow;

        // Update H flag
        flag_update(HF,(reg[reg_id] & 0xf0) == (total & 0xf0));

        // Update C flag
        flag_update(CF,!(total < 0));

        // Set N flag
        flag_update(NF,1);

        // Update 8 bit register
        reg[reg_id] -= sub_value - borrow;

        // Update Z flag
        flag_update(ZF,(reg[reg_id]==0));
    }
    else
    {
        throw "Invalid register id";
    }
}

// Add 2 16-bit registers and update flags
void Memory::reg_add16(uint8_t reg_id, uint16_t reg_n_val)
{
    uint32_t total;

    // The total sum
    total = reg_get16(reg_id) + reg_n_val;

    // Update H flag (half carry from bit 11)
    flag_update(HF,(reg_get16(reg_id) & 0xf000) == (total & 0xf000));

    // Update C flag (carry from bit 15)
    flag_update(CF,(total > 0xffff));

    // Clear N flag
    flag_update(NF,0);

    // Update 16 bit register
    reg_set(reg_id,(uint16_t)total);

}

// Decimal Adjust register A (DAA), flags updated
void Memory::reg_daa()
{
    // Check if ls digit is valid BCD value
    if ((reg[RA] & 0xf) > 9 || flag_get(HF))
    {
        reg[RA] += 6;
    }
    // Check if ms digit is valid BCD value
    if ((reg[RA] & 0xf0) > 9 || flag_get(CF))
    {
        reg[RA] += 0x60;

        // Update C flag
        flag_update(CF,1);
    }
    // Update H flag
    flag_update(HF,0);

    // Update Z flag
    flag_update(ZF,(reg[RA]==0));
}

// Rotate contents of A register left and store bit 7 in CF, flags updated
// When carry is true, bit 0 = previous bit 7
// When carry is false, bit 0 = previous CF
// Flags 0 0 0 C
void Memory::reg_rla(bool carry)
{
    uint8_t a_value, bit7;

    // Get value of bit 7
    bit7 = (reg[RA] & 0x80) >> 7;

    // Shift register A left by 1
    a_value = reg[RA] << 1;

    if (carry)
    {
        // Append previous bit 7 to bit 0
        reg[RA] = a_value + bit7;
    }
    else
    {
        // Append previous CF to bit 0
        reg[RA] = a_value + flag_get(CF);
    }

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,0);

    // Set CF to bit7
    flag_update(CF,(bool)bit7);
}

// Rotate contents of register n left and store bit 7 in CF, flags updated
// When carry is true, bit 0 = previous bit 7
// When carry is false, bit 0 = previous CF
// Flags Z 0 0 C
void Memory::reg_rl(uint8_t reg_id, bool carry)
{
    uint8_t a_value, bit7;

    // Get value of bit 7
    bit7 = (reg[reg_id] & 0x80) >> 7;

    // Shift register A left by 1
    a_value = reg[reg_id] << 1;

    if (carry)
    {
        // Append previous bit 7 to bit 0
        reg[reg_id] = a_value + bit7;
    }
    else
    {
        // Append previous CF to bit 0
        reg[reg_id] = a_value + flag_get(CF);
    }

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(reg[reg_id]==0));

    // Set CF to bit7
    flag_update(CF,(bool)bit7);
}

// Rotate contents of A register right and store bit 0 in CF, flags updated
// When carry is true, bit 7 = previous bit 0
// When carry is false, bit 7 = previous CF
// Flags 0 0 0 C
void Memory::reg_rra(bool carry)
{
    uint8_t a_value, bit0;

    // Get value of bit 0
    bit0 = reg[RA] & 0x01;

    // Shift register A right by 1
    a_value = reg[RA] >> 1;

    if (carry)
    {
        // Append previous bit 0 to bit 7
        reg[RA] = a_value + (bit0 << 7);
    }
    else
    {
        // Append previous CF to bit 7
        reg[RA] = a_value + (flag_get(CF) << 7);
    }

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,0);

    // Set CF to bit0
    flag_update(CF,(bool)bit0);
}

// Rotate contents of register n right and store bit 0 in CF, flags updated
// When carry is true, bit 7 = previous bit 0
// When carry is false, bit 7 = previous CF
// Flags Z 0 0 C
void Memory::reg_rr(uint8_t reg_id, bool carry)
{
    uint8_t a_value, bit0;

    // Get value of bit 0
    bit0 = reg[reg_id] & 0x01;

    // Shift register A right by 1
    a_value = reg[reg_id] >> 1;

    if (carry)
    {
        // Append previous bit 0 to bit 7
        reg[reg_id] = a_value + (bit0 << 7);
    }
    else
    {
        // Append previous CF to bit 7
        reg[reg_id] = a_value + (flag_get(CF) << 7);
    }

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(reg[reg_id]==0));

    // Set CF to bit0
    flag_update(CF,(bool)bit0);
}

// Shift contents of register n left and store bit 7 in CF, bit0 = 0, flags updated
// Flags Z 0 0 C
void Memory::reg_sla(uint8_t reg_id)
{
    uint8_t bit7;

    // Get value of bit 7
    bit7 = (reg[reg_id] & 0x80) >> 7;

    // Shift register left by 1
    reg[reg_id] = reg[reg_id] << 1;

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(reg[reg_id]==0));

    // Set CF to bit7
    flag_update(CF,(bool)bit7);
}

// Shift contents of register A right and store bit 0 in CF, bit7 unchanged, flags updated
// Flags Z 0 0 C
void Memory::reg_sra(uint8_t reg_id)
{
    uint8_t bit0, bit7;

    // Get value of bit 0
    bit0 = reg[reg_id] & 0x01;

    // Get value of bit 7
    bit7 = reg[reg_id] & 0x80;

    // Shift register right by 1 and reinstate bit 7
    reg[reg_id] = (reg[reg_id] >> 1) + bit7;

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(reg[reg_id]==0));

    // Set CF to bit0
    flag_update(CF,(bool)bit0);
}

// Shift contents of register A right and store bit 0 in CF, bit7 = 0, flags updated
// Flags Z 0 0 C
void Memory::reg_srl(uint8_t reg_id)
{
    uint8_t bit0;

    // Get value of bit 0
    bit0 = reg[reg_id] & 0x01;

    // Shift register right by 1
    reg[reg_id] = reg[reg_id] >> 1;

    // Update H flag
    flag_update(HF,0);

    // Update N flag
    flag_update(NF,0);

    // Update Z flag
    flag_update(ZF,(reg[reg_id]==0));

    // Set CF to bit0
    flag_update(CF,(bool)bit0);
}
