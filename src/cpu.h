#ifndef CPU_H
#define CPU_H

#include "memory.h"

class Cpu
{

    public:
        Cpu();                              // Constructor
        void attach_memory(Memory* mem_in);
        void cycle();                       // Perform a single CPU cycle

    private:
        Memory* mem;                        // Pointer to memory object
        uint8_t byte_in;                    // Current byte pointed to by sp
};

#endif // CPU_H
