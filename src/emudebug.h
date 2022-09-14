#pragma once

#include "memory.h"
#include <stdint.h>

class Emudebug
{
    public:
        Emudebug(Memory& mem_in);
        void dump_reg();
        bool detect_runaway();

        void ram_debug(uint16_t start_addr);
        void ram_debug(uint16_t start_addr1, uint16_t start_addr2);
        void insert_logo();
        void sprite_test();

    private:
        Memory& mem;
        uint8_t prev_pc, cpu_count;
        char rdb[1000];                                             // 1000 bytes of debug ram
        char rdb2[1000];                                            // 1000 bytes of debug ram
};