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
    // Initialise registers, sp and pc
    sp = SP_INITIAL_VALUE;    // sp initialised to fffe (gameboy default)
    pc = ROM_START_ADDRESS;   // pc initialised to 0x100 (start address)
    flags = 0;
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
    printf("register %i set to %0.2X\n", reg_id, reg_value); // Debug message
}

// Set value in 16 bit register
void Memory::reg_set(uint8_t reg_id, uint16_t reg_value)
{
    // Make sure reg_id is valid
    if (reg_id < REG_ARRAY_SIZE)
    {
        // to be implemented
    }
}

// Copy data between 8 or 16 bit registers
// 16-bit pointers can be used as reg_id
// for now assuming to register is 8-bit
void Memory::reg_copy(uint8_t to_reg_id, uint8_t from_reg_id)
{
    // Check what type of register is used for from_reg
    if (from_reg_id < 8)
    {
        // use reg_get and reg_set to copy data
        reg_set(to_reg_id, reg_get(from_reg_id));
    }
    else if ((from_reg_id > 11) && (from_reg_id < 16))
    {
        // 16-bit pointer register
        reg_set(to_reg_id,get_from_pointer(from_reg_id));
    }
}

// Read byte from RAM/ROM
uint8_t Memory::read_byte(uint16_t address)
{
    return ram[address];
}

// Read byte from RAM/ROM pointed to by 16-bit register
uint8_t Memory::get_from_pointer(uint8_t reg_id)
{
    return read_byte(reg_get16(reg_id));
}

// Read byte from ROM and increment PC
uint8_t Memory::fetch_byte()
{
    // Need to return current byte before incrementing PC
    return ram[pc++];
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

// Increment pc by amount
void Memory::inc_pc(int8_t amount)
{
    // Increment the pc
    pc += amount;
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

// Function to get the current ROM title
string Memory::get_rom_title()
{
    return rom_title;
}
