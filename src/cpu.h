#pragma once

#include "memory.h"
#include "clock.h"
#include "interrupt.h"
#include "emudebug.h"
#include <stdint.h>

class Cpu
{

    public:
        Cpu(Memory& mem, Clock& clk, Interrupt& ir, Emudebug& edb);
        void cycle();

    private:
        void process_instruction(uint8_t rom_byte);
        void process_cb_instruction(uint8_t rom_byte);

        Memory& mem;
        Clock& clk;
        Interrupt& ir;
        Emudebug& edb;
        uint8_t byte_in;
};