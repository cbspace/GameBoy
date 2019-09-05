#include "memory.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

/// Stack pointer and Program Counter

// Set pc value
void Memory::set_pc(uint16_t pc_value)
{
    pc = pc_value;
}

// Get pc value
uint16_t Memory::get_pc()
{
    return pc;
}

// Set sp value
void Memory::set_sp(uint16_t sp_value)
{
    sp = sp_value;
}

// Get sp value
uint16_t Memory::get_sp()
{
    return sp;
}

// Increment pc by amount
void Memory::inc_pc(uint8_t amount)
{
    // Increment the pc
    pc += amount;
}

// Decrement pc by amount
void Memory::dec_pc(uint8_t amount)
{
    // Increment the pc
    pc -= amount;
}

// Increment sp by amount
void Memory::inc_sp(uint8_t amount)
{
    // Increment the pc
    sp += amount;
}

// Decrement sp by amount
void Memory::dec_sp(uint8_t amount)
{
    // Increment the pc
    sp -= amount;
}

// Add value to sp and update flags
void Memory::sp_add(uint8_t amount)
{
    uint32_t total;

    // The total sum
    total = sp + amount;

    // Update H flag (half carry from bit 11)
    flag_update(HF,(sp & 0xf000) == (total & 0xf000));

    // Update C flag (carry from bit 15)
    flag_update(CF,(total > 0xffff));

    // Clear N flag
    flag_update(NF,0);

    // Clear Z flag
    flag_update(ZF,0);

    // Update 16 bit sp register
    sp = (uint16_t)total;
}

// Push 16-bit value to stack and decrement sp
void Memory::stack_push(uint16_t push_val)
{
    uint8_t byte_val;

    // Push high byte on first then lower byte
    byte_val = push_val >> 8;
    ram[sp] = byte_val;
    byte_val = push_val & 0xff;
    ram[sp-1] = byte_val;

    // Decrement the stack pointer
    dec_sp(2);
}

// Push PC value on to stack and decrement sp
void Memory::pc_push()
{
    uint8_t byte_val;

    // Push high byte on first then lower byte
    byte_val = pc >> 8;
    ram[sp] = byte_val;
    byte_val = pc & 0xff;
    ram[sp-1] = byte_val;

    // Decrement the stack pointer
    dec_sp(2);
}

// Pop 16-bit value from stack and increment sp
uint16_t Memory::stack_pop()
{
    uint16_t ret_val;

    // Pop high byte first then lower byte
    ret_val = (ram[sp+2] << 8) + (ram[sp+1] & 0xff);

    // Increment the stack pointer
    inc_sp(2);

    return ret_val;
}

// Jump to address at PC + e (e = signed 8-bit immediate)
void Memory::jmp_n(int8_t jmp_amount)
{
    int32_t pc_val;

    // Set pc to new value
    pc_val = (int32_t)pc + (int32_t)jmp_amount;
    pc = (uint16_t)pc_val;
}
