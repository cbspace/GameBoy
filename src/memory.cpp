#include "memory.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

// Constructor
Memory::Memory()
{
    rom_title = "";
}

// Function to load a ROM file (currently only supporting
// 16 or 32kb ROMS - cart type 0 or 1)
int Memory::load_rom(char* rom_path)
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
