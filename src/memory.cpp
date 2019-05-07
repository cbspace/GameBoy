#include "memory.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <array>
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
    sp = 0xfffe;    // sp initialised to fffe (gameboy default)
    pc = 0x100;     // pc initialised to 0x100 (start address)
    flags = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        reg[i] = 0;
    }
}

// Read byte from RAM/ROM
uint8_t Memory::read_byte(uint16_t address)
{
    return ram[address];
}

// Read array of bytes from RAM/ROM
// Max length is currently 14 bytes to match game title string
array<uint8_t, BYTE_ARRAY_SIZE> Memory::read_array(uint16_t address, uint8_t length)
{
    // Copy bytes
    array<uint8_t, BYTE_ARRAY_SIZE> byte_array;
    for (uint8_t i = 0; i < length; i++)
    {
        byte_array[i] = ram[address + i];
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
            rom_title = ram + 0x134;
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
