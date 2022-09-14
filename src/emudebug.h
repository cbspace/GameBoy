#pragma once

#include "memory.h"
#include <stdint.h>

class Emudebug
{
    public:
        Emudebug(Memory& mem_in);
        void dump_reg();
        bool detect_runaway();

    private:
        Memory& mem;
        uint8_t prev_pc, cpu_count;
};