#include "cpu.h"
#include "../memory/memory.h"
#include "../clock/clock.h"
#include "../interrupt/interrupt.h"
#include "../emudebug/emudebug.h"
#include <SDL2/SDL.h>

void Cpu::cycle()
{
    uint8_t byte_in;

    if (!ir->get_halt() && !ir->get_stop())
    {
         ///temp
//        printf("PC: %4X\n", mem->get_pc());
//        if (mem->get_pc() == 0x006a)
//        {
//           SDL_Delay(1); //Dummy put break point here
//        }
//        edb->dump_reg();
//        mem->ram_debug(0x8010);

        byte_in = mem->fetch_byte();

        ///temp
        //printf("Byte: %2X\n\n", byte_in);

        process_instruction(byte_in);
    }
    else if (ir->get_halt())
    {
        clk->add_cycles(4);
    }
}

Cpu::Cpu()
{
    mem = NULL;
    ir = NULL;
    clk = NULL;
    edb = NULL;
    byte_in = 0x00;
}

void Cpu::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

void Cpu::attach_clock(Clock* clock_in)
{
    clk = clock_in;
}

void Cpu::attach_interrupt(Interrupt* interrupt_in)
{
    ir = interrupt_in;
}

void Cpu::attach_emudebug(Emudebug* debug_in)
{
    edb = debug_in;
}
