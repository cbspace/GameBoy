#include "interrupt.h"
#include "../memory/memory.h"
#include "../clock/clock.h"

#include <iostream>

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
    //disp = NULL;
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
            // Reset IME
            ime = false;

            // Push PC onto stack
            mem->pc_push();

            if (I_VBLANK & i_current)
            {
            	// Reset Interrupt Flag
                if_update(I_VBLANK, false);

            	// Jump to starting address
                mem->set_pc(IV_VBLANK);

                //cout << "VBlank Interrupt\n";
            }
            else if (I_LCDSTAT & i_current)
            {
            	cout << "LCDStat Interrupt\n";

                // Reset Interrupt Flag
                if_update(I_LCDSTAT, false);

            	// Jump to starting address
                mem->set_pc(IV_LCDSTAT);
            }
            else if (I_JOYPAD & i_current)
            {
            	cout << "Joypad Interrupt\n";

                // Reset Interrupt Flag
                if_update(I_JOYPAD, false);

            	// Jump to starting address
                mem->set_pc(IV_JOYPAD);
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

// Enable interrupts
void Interrupt::enable_interrupts()
{
	// Set flag used to delay setting of IME
	ei_count = 1;
}

//------------------------- Private ----------------------------------------

// Exit stopped status
void Interrupt::cancel_stop()
{
    stop_flag = false;
    clk->add_cycles(217);
}

// Process ei_count
void Interrupt::process_ei_count()
{
    // Process the EI counter (used to delay EI instruction 1 cycle)
    // 0 = Normal State, 1 = Flag Set, 2 = Delayed 1 cycle ready to reset
    if (ei_count == 0)
    {
        // Enable Interrupts
        ime = true;
    }
    else if (ei_count == 1)
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
