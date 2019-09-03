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

/// Flag Operations

// Set or clear a flag
void Memory::flag_update(uint8_t flag_id, uint8_t flg_val)
{
    if (flg_val!=0)
    {
        // Set flag to 1
        reg[RF] |= flag_id;
    }
    else
    {
        // Clear flag to 0
        reg[RF] &= ~flag_id;
    }
}

// Return flag value
uint8_t Memory::flag_get(uint8_t flag_id)
{
    uint8_t flag_val;
    flag_val = (reg[RF] & flag_id);
    return !(flag_val==0);
}

/// ROM Cartridge and game title

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

// Copy 256 bytes from start address to debug ram
void Memory::ram_debug(uint16_t start_addr)
{
    for (uint16_t i = 0; i < 256; i++)
    {
        rdb[i] = ram[start_addr + i];
    }
}

// For testing purposes add the N logo into the RAM
void Memory::debug_insert_logo()
{
    uint8_t logo_data[] = {0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f,
    0x88, 0x89, 0x00, 0x0e, 0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
    0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f,
    0xbb, 0xb9, 0x33, 0x3e};

    for (uint16_t i = 0; i < 48; i++)
    {
        ram[i + 0x104] = logo_data[i];
    }
}
