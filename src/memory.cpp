#include "memory.h"
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

Memory::Memory()
{
    init();
}

void Memory::init()
{
    set_sp(SP_INITIAL_VALUE);    // sp initialised to fffe (gameboy default)
    set_pc(ROM_START_ADDRESS);   // pc initialised to 0x100 (start address)

    for (uint8_t i = 0; i < REG_ARRAY_SIZE; i++)
    {
        reg[i] = 0;
    }

    for (uint32_t i = 0; i < MEM_SIZE; i++)
    {
        ram[i] = 0;
    }
}

/// Flag Operations

// Set or clear a flag
void Memory::flag_update(uint8_t flag_id, uint8_t flg_val)
{
    if (flg_val!=0)
    {
        reg[RF] |= flag_id;
    }
    else
    {
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

optional<Error> Memory::load_rom(char* rom_path)
{
    streampos file_size;

    ifstream file(rom_path, ios::in|ios::binary|ios::ate);
    if (!file.is_open()) { return Error("Unable to open file",rom_path); }

    file_size = file.tellg();
    if (file_size > 32*1024) { return Error("ROM not supported, up to 32kb support only"); }

    file.seekg(0, ios::beg);
    file.read(ram, file_size);
    file.close();
    read_rom_title();

    return nullopt;
}

void Memory::read_rom_title()
{
    char rom_title_array[BYTE_ARRAY_SIZE];

    // Copy bytes (max length for title is 14 bytes)
    for (uint8_t i = 0; i < 14; i++)
    {
        rom_title_array[i] = ram[ROM_TITLE_ADDRESS + i];

        if (ram[ROM_TITLE_ADDRESS + i] == '\0') {
            break;
        } 
    }

    rom_title = rom_title_array;
}

string Memory::get_rom_title()
{
    return rom_title;
}
