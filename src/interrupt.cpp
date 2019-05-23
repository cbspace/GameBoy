#include "interrupt.h"
#include "memory.h"
#include "clock.h"

Interrupt::Interrupt()
{
    ime = true;             // Turn on interrupt master enable
    halt_flag = false;      // Default is not halted
    stop_flag = false;      // Default is not stopped
}

// Set pointer used to access memory object
void Interrupt::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

// Set pointer used to access clock object
void Interrupt::attach_clock(Clock* clk_in)
{
    clk = clk_in;
}

// Halt the CPU until an interrupt occurs (HALT)
void Interrupt::cpu_halt()
{
    if (ime)
    {
        halt_flag = true;
    }
}

// Stop the CPU and LCD until a button is pressed (STOP)
void Interrupt::cpu_stop()
{
    stop_flag = true;
}

// Get value of CPU halt flag
bool Interrupt::get_halt()
{
    return halt_flag;
}

// Get value of CPU stop flag
bool Interrupt::get_stop()
{
    return stop_flag;
}

// Exit stopped status
void Interrupt::cancel_stop()
{
    stop_flag = false;
    clk->add_cycles(217);
}
