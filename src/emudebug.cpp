#include "emudebug.h"
#include "memory.h"
#include <stdint.h>
#include <iostream>
#include <iomanip>

using namespace std;

Emudebug::Emudebug(Memory& mem_in) :
	mem(mem_in)
{
    cpu_count = 0;
    prev_pc = 0;
}

bool Emudebug::detect_runaway()
{
	uint16_t pc_val;

	pc_val = mem.get_pc();

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
    cout << "PC: " << hex << setfill('0') << setw (4) << mem.get_pc() << endl;
    cout << "A/F: " << hex << setfill('0') << setw (4) << mem.reg_get16(RAF) << endl;
    cout << "B/C: " << hex << setfill('0') << setw (4) << mem.reg_get16(RBC) << endl;
    cout << "D/E: " << hex << setfill('0') << setw (4) << mem.reg_get16(RDE) << endl;
    cout << "H/L: " << hex << setfill('0') << setw (4) << mem.reg_get16(RHL) << endl;
    cout << endl;
}

void Emudebug::ram_debug(uint16_t start_addr)
{
    for (uint16_t i = 0; i < 1000; i++)
    {
        rdb[i] = mem.get_byte(start_addr + i);
    }
}

void Emudebug::ram_debug(uint16_t start_addr1, uint16_t start_addr2)
{
    for (uint16_t i = 0; i < 1000; i++)
    {
        rdb[i] = mem.get_byte(start_addr1 + i);
        rdb2[i] = mem.get_byte(start_addr2 + i);
    }

}

void Emudebug::insert_logo()
{
    uint8_t logo_data[] = {0xce, 0xed, 0x66, 0x66, 0xcc, 0x0d, 0x00, 0x0b,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0c, 0x00, 0x0d, 0x00, 0x08, 0x11, 0x1f,
    0x88, 0x89, 0x00, 0x0e, 0xdc, 0xcc, 0x6e, 0xe6, 0xdd, 0xdd, 0xd9, 0x99,
    0xbb, 0xbb, 0x67, 0x63, 0x6e, 0x0e, 0xec, 0xcc, 0xdd, 0xdc, 0x99, 0x9f,
    0xbb, 0xb9, 0x33, 0x3e};

    for (uint16_t i = 0; i < 48; i++)
    {
        mem.write_byte(i + 0x104, logo_data[i]);
    }
}

void Emudebug::sprite_test()
{
    uint8_t sprite_data[] = {0x7c, 0x7c, 0x00, 0xc6, 0xc6, 0x00, 0x00, 0xfe,
    						 0xc6, 0xc6, 0x00, 0xc6, 0xc7, 0x01, 0x7c, 0x7c};
//    uint8_t sprite_data[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
//    		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

    for (uint16_t i = 0; i < 16; i++)
    {
		mem.write_byte(i + A_TDT1 + 0x230, sprite_data[i]);
    }

    uint8_t oam_data[] = {8, 8, 35, 0x00};

    for (uint16_t i = 0; i < 4; i++)
    {
        mem.write_byte(i + A_OAM, oam_data[i]);
    }

    uint8_t oam_data2[] = {160, 144, 35, 0x00};

    for (uint16_t i = 0; i < 4; i++)
    {
		mem.write_byte(i + A_OAM + 4, oam_data2[i]);
    }
}