#include "memory.h"
#include <fstream>
#include <iostream>
#include <string>
using namespace std;

Memory::Memory()
{
    // Create RAM array
    ram = new char[MEM_SIZE];
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
            cout << "Rom Title: " << ram + 0x134 << endl;
        } else {
            printf("ROM not supported, up to 32kb support only\n");
        }
    } else {
        printf("Unable to open file '%s'\n", rom_path);
    }

    return 0;
}
