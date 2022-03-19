#ifndef EMUDEBUG_H
#define EMUDEBUG_H

#include "../memory/memory.h"
#include <stdint.h>

class Emudebug
{
    public:
        Emudebug();
        void attach_memory(Memory* mem_in);                  // Set pointer used to access memory object
        void dump_reg();                                     // Dump PC, stack and registers
        void dump_mem(uint8_t start, uint8_t finish);        // Dump memory from start to finish
        bool detect_runaway(); 							     // Detect runaway CPU

    private:
        Memory* mem;                                         // Pointer to memory object

        uint8_t prev_pc, cpu_count;						     // Count sequencial PC values
};

#endif // EMUDEBUG_H
