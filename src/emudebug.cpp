#include "emudebug.h"
#include "memory.h"
#include <stdint.h>
#include <iostream>
#include <iomanip>

using namespace std;

// Dump PC, stack and registers
void Emudebug::dump_reg()
{
    cout << endl;
    cout << " --- REG DUMP ---" << uppercase << endl;
    cout << "PC: " << hex << setfill('0') << setw (4) << mem->get_pc() << endl;
    cout << "A/F: " << hex << setfill('0') << setw (4) << mem->reg_get16(RAF) << endl;
    cout << "B/C: " << hex << setfill('0') << setw (4) << mem->reg_get16(RBC) << endl;
    cout << "D/E: " << hex << setfill('0') << setw (4) << mem->reg_get16(RDE) << endl;
    cout << "H/L: " << hex << setfill('0') << setw (4) << mem->reg_get16(RHL) << endl;
    cout << endl;
}

// Dump memory from start to finish
void Emudebug::dump_mem(uint8_t start, uint8_t finish)
{

}

// Constructor
Emudebug::Emudebug()
{
    mem = NULL;
}

// Set pointer used to access memory object
void Emudebug::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}
