#pragma once

#include "Memory.h"
#include "Clock.h"
#include "interrupt.h"
#include "emudebug.h"
#include "lib/Types.h"

class Cpu
{

    public:
        Cpu(Memory& mem_in, Clock& clk_in, Interrupt& ir_in, Emudebug& edb_in);
        void cycle();

    private:
        void process_instruction(u8 rom_byte);
        void process_cb_instruction(u8 rom_byte);

        Memory& mem;
        Clock& clk;
        Interrupt& ir;
        Emudebug& edb;
        u8 byte_in;
};