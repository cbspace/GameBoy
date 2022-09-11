#include "emudebug.h"
#include "../memory/memory.h"
#include <stdint.h>
#include <iostream>
#include <iomanip>

using namespace std;

Emudebug::Emudebug()
{
    mem = NULL;
    cpu_count = 0;
    prev_pc = 0;
}

bool Emudebug::detect_runaway()
{
	uint16_t pc_val;

	pc_val = mem->get_pc();

	if (pc_val == (prev_pc + 1))
	{
		cpu_count += 1;
		if (cpu_count >= 100)
		{
			cout << "CPU Runaway detected!\n";
			return true;
		}
	}
	else
	{
		cpu_count = 0;
	}

	prev_pc = pc_val;

	return false;
}

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

void Emudebug::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}
