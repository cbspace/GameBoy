#include "interrupt.h"
#include "memory.h"
#include "clock.h"
#include "display.h"

Interrupt::Interrupt()
{
    ime = true;             // Turn on interrupt master enable
    halt_flag = false;      // Default is not halted
    stop_flag = false;      // Default is not stopped
    ei_count = 0;           // 0 = Normal State, 1 = Flag Set, 2 = Delayed 1 cycle ready to reset
    i_flags = 0;
    i_enable = 0;
    mem = NULL;
    clk = NULL;
    disp = NULL;
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

// Set pointer used to access display object
void Interrupt::attach_display(Display* disp_in)
{
    disp = disp_in;
}

// Perform interrupt checks each cycle
void Interrupt::check_interrupts()
{
    // Current interrups
    uint8_t i_current;

    // Interrupt Master Enable
    if (ime)
    {
        // Get IF and IE values
        i_flags = mem->get_byte(A_IFLAGS);
        i_enable = mem->get_byte(A_IENABLE);
        i_current = i_flags & i_enable;

        // Check for interrupt
        if (i_current)
        {
            if (I_VBLANK & i_current)
            {
                //printf("VBLank Interrupt\n");

                // Draw the frame!
                //disp->update_frame();
            }
        }
    }

    // If ei_count is not == 0 then process the counter
    if (ei_count)
    {
        process_ei_count();
    }
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

// Disable interrupts
void Interrupt::disable_interrupts()
{
    ime = false;
}

//------------------------- Private ----------------------------------------

// Exit stopped status
void Interrupt::cancel_stop()
{
    stop_flag = false;
    clk->add_cycles(217);
}

// Enable interrupts
void Interrupt::enable_interrupts()
{
    // Make sure that DI command wasnt used in previous cycle
    if (ei_count == 0)
    {
        // Enable Interrupts
        ime = true;
    }
}

// Process ei_count
void Interrupt::process_ei_count()
{
    // Process the EI counter (used to delay EI instruction 1 cycle)
    // 0 = Normal State, 1 = Flag Set, 2 = Delayed 1 cycle ready to reset
    if (ei_count == 1)
    {
        ei_count++;
    }
    else if (ei_count == 2)
    {
        ei_count = 0;
        ime = true;
    }
}

// Return flag value
bool Interrupt::if_get(uint8_t int_flag)
{
    uint8_t flag_val;
    flag_val = mem->get_byte(A_IFLAGS) & int_flag;
    return !(flag_val==0);
}

// Set or clear an interrupt flag
void Interrupt::if_update(uint8_t int_flag, bool flg_val)
{
    if (flg_val)
    {
        i_flags |= int_flag;
    }
    else
    {
        i_flags &= ~int_flag;
    }

    // Update interrupt flags
    mem->write_byte(A_IFLAGS,i_flags);
}
