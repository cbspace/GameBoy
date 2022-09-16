#pragma once

#include "memory.h"
#include "clock.h"
#include "interrupt.h"
#include "emudebug.h"
#include "lib/Types.h"

class Cpu
{

    public:
        Cpu(Memory& mem, Clock& clk, Interrupt& ir, Emudebug& edb);
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