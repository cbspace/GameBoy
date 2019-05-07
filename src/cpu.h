#ifndef CPU_H
#define CPU_H

#include "memory.h"
#include "clock.h"

class Cpu
{

    public:
        Cpu();                              // Constructor
        void attach_memory(Memory* mem_in); // Set pointer used to access memory object
        void attach_clock(Clock* clock_in); // Set pointer used to access clock object
        void cycle();                       // Perform a single CPU cycle

    private:
        Memory* mem;                        // Pointer to memory object
        Clock* clk;                         // Pointer to clock object
        uint8_t byte_in;                    // Current byte pointed to by sp
};

#endif // CPU_H
