#include "cpu.h"
#include "../memory/memory.h"
#include "../clock/clock.h"
#include "../interrupt/interrupt.h"
#include "../emudebug/emudebug.h"
#include <SDL2/SDL.h>

// Perform a single CPU cycle
void Cpu::cycle()
{
    // Byte read from ROM
    uint8_t byte_in;

    // Check if CPU is halted
    if (!ir->get_halt() && !ir->get_stop())
    {
        ///temp
//        printf("PC: %4X\n", mem->get_pc());
//        if (mem->get_pc() == 0x006a)
//        {
//            SDL_Delay(1); //Dummy put break point here
 //       }
        //edb->dump_reg();
//        mem->ram_debug(0x8010);

        // Fetch byte for execution
        byte_in = mem->fetch_byte();

        ///temp
        //printf("Byte: %2X\n\n", byte_in);

        // Process the byte
        process_instruction(byte_in);
    }
    else if (ir->get_halt())
    {
        // The CPU is halted but we need to continue cycling
        clk->add_cycles(4);
    }
}

// Constructor
Cpu::Cpu()
{
    mem = NULL;
    ir = NULL;
    clk = NULL;
    edb = NULL;
    byte_in = 0x00;
}

// Set pointer used to access memory object
void Cpu::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

// Set pointer used to access memory object
void Cpu::attach_clock(Clock* clock_in)
{
    clk = clock_in;
}

// Set pointer used to access interrupt object
void Cpu::attach_interrupt(Interrupt* interrupt_in)
{
    ir = interrupt_in;
}

// Set pointer used to access debug object
void Cpu::attach_emudebug(Emudebug* debug_in)
{
    edb = debug_in;
}
