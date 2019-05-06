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
    sp = 0xfffe;    // sp initialised to fffe (gameboy default)
    pc = 0x100;     // pc initialised to 0x100 (start address)
    flags = 0;
    for (uint8_t i = 0; i < 4; i++)
    {
        reg[i] = 0;
    }
}

// Get byte pointed to by pc
int8_t Memory::get_current_byte()
{
    // Return the byte
    return ram[pc];
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
