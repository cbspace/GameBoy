#include "memory.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// Constructor
Memory::Memory()
{
    rom_title = "";
    init();
}

// Initialise memory
void Memory::init()
{
    // Initialise sp and pc
    set_sp(SP_INITIAL_VALUE);    // sp initialised to fffe (gameboy default)
    set_pc(ROM_START_ADDRESS);   // pc initialised to 0x100 (start address)

    // Initialise registerse
    for (uint8_t i = 0; i < REG_ARRAY_SIZE; i++)
    {
        reg[i] = 0;
    }
}

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
    printf("register %i set to %0.2X\n", reg_id, reg_value); // Debug message
}

// Set value in 16 bit register
void Memory::reg_set(uint8_t reg_id, uint16_t reg_value)
{
    uint8_t high_byte;
    uint8_t low_byte;

    high_byte = reg_value >> 8;
    low_byte = reg_value & 0xff;

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
        flag_update(HF,(reg[reg_id] & 0xf0 == (reg[reg_id] + 1) & 0xf0));

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

// Decrement register, flags not changed
void Memory::reg_dec(uint8_t reg_id)
{
    // 16 bit register value
    uint16_t reg_val;

    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        // 8 bit register
        reg[reg_id]--;
    }
    else if ((reg_id > REG_16_START) && (reg_id < REG_16_END)) // Check if valid 16-bit register used
    {
        reg_val = reg_get16(reg_id) - 1;
        reg_set(reg_id,reg_val);
    }
    else
    {
        throw "Invalid register id";
    }
}

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
        flag_update(HF,(reg[reg_id] & 0xf0 == total & 0xf0));

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

// Add value to 16-bit register and update flags, bool parameter is add with carry
void Memory::reg_add(uint8_t reg_id, uint16_t add_value)
{
    // 16 bit register value
    uint16_t reg_val;

    // Check if valid 16-bit register used
    if ((reg_id > REG_16_START) && (reg_id < REG_16_END))
    {
        reg_val = reg_get16(reg_id) + add_value;
        reg_set(reg_id,reg_val);
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
        flag_update(HF,(reg[reg_id] & 0xf0 == total & 0xf0));

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

// Subtract value from 16-bit register, flags updated
void Memory::reg_sub(uint8_t reg_id, uint16_t sub_value)
{
    // 16 bit register value
    uint16_t reg_val;

    // Check if valid 16-bit register used
    if ((reg_id > REG_16_START) && (reg_id < REG_16_END))
    {
        reg_val = reg_get16(reg_id) - sub_value;
        reg_set(reg_id,reg_val);
    }
    else
    {
        throw "Invalid register id";
    }
}

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
    flag_update(HF,(reg[RA] & 0xf0 == total & 0xf0));

    // Update C flag
    flag_update(CF,!(total < 0));

    // Update Z flag
    flag_update(ZF,(total==0));

    // Set N flag
    flag_update(NF,1);
}

// Read byte from RAM/ROM
uint8_t Memory::get_byte(uint16_t address)
{
    return ram[address];
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
}

// Write array of bytes to RAM/ROM
void Memory::write_array(uint16_t address, uint8_t* bytes, uint8_t length)
{
        // Copy bytes
        for (uint8_t i = 0; i < length; i++)
        {
            ram[i] = ram[address + i];
        }
}

// Set pc value
void Memory::set_pc(uint16_t pc_value)
{
    pc = pc_value;
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

// Push 16-bit value to stack and decrement sp
void Memory::stack_push(uint16_t push_val)
{
    uint8_t byte_val;

    // Decrement the stack pointer
    dec_sp(2);

    // Push high byte on first then lower byte
    byte_val = push_val >> 8;
    ram[sp+1] = byte_val;
    byte_val = push_val & 0xff;
    ram[sp] = byte_val;
}

// Pop 16-bit value from stack and increment sp
uint16_t Memory::stack_pop()
{
    uint16_t ret_val;

    // Increment the stack pointer
    inc_sp(2);

    // Pop high byte first then lower byte
    ret_val = ram[sp-1] << 8;
    ret_val += ram[sp];

    return ret_val;
}

// Set or clear a flag
void Memory::flag_update(uint8_t flag_id, uint8_t flg_val)
{
    if (flg_val)
    {
        reg[RF] |= flag_id;
    }
    else
    {
        reg[RF] ^= !flag_id;
    }
}

// Return flag value
bool Memory::flag_get(uint8_t flag_id)
{
    uint8_t flag_val;
    flag_val = reg[RF] & flag_id;
    return !(flag_val==0);
}

// Function to load a ROM file (currently only supporting
// 16 or 32kb ROMS - cart type 0 or 1)
int8_t Memory::load_rom(char* rom_path)
{
    streampos file_size;

    ifstream file(rom_path, ios::in|ios::binary|ios::ate);
    if (file.is_open()) {
        file_size = file.tellg();
        if (file_size <= 32*1024) {
            file.seekg(0, ios::beg);
            file.read(ram, file_size);
            file.close();
            read_rom_title();
        } else {
            cout << "ROM not supported, up to 32kb support only" << endl;
        }
    } else {
        cout << "Unable to open file '" << rom_path << "'" << endl;
    }

    return 0;
}

// Read rom title and load into string
void Memory::read_rom_title()
{
    char rom_title_array[BYTE_ARRAY_SIZE];

    // Copy bytes (max length for title is 14 bytes)
    for (uint8_t i = 0; i < 14; i++)
    {
        rom_title_array[i] = ram[ROM_TITLE_ADDRESS + i];
    }

    // Append a Null
    rom_title_array[14] = '\0';

    // Convert to a string
    rom_title = rom_title_array;
}

// Function to get the current ROM title
string Memory::get_rom_title()
{
    return rom_title;
}
