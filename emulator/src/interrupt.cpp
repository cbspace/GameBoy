#include "interrupt.h"
#include "Memory.h"
#include "Clock.h"

Interrupt::Interrupt(Memory& mem_in) :
    mem(mem_in)
{
    ime = true;             // Turn on interrupt master enable
    ei_count = 0;           // 0 = Normal State, 1 = Flag Set, 2 = Delayed 1 cycle ready to reset
    i_flags = 0;
    i_enable = 0;
}

void Interrupt::check_interrupts()
{
    u8 i_current;

    if (ime)
    {
        i_flags = mem.get_byte(A_IFLAGS);
        i_enable = mem.get_byte(A_IENABLE);
        i_current = i_flags & i_enable;

        if (i_current)
        {
            ime = false;

            mem.pc_push();

            if (I_VBLANK & i_current)
            {
                if_update(I_VBLANK, false);

                mem.set_pc(IV_VBLANK);
                cout << "Entering V-Blank" << endl;

            }
            else if (I_LCDSTAT & i_current)
            {
                cout << "LCDStat Interrupt" << endl;

                if_update(I_LCDSTAT, false);

                mem.set_pc(IV_LCDSTAT);
            }
            else if (I_JOYPAD & i_current)
            {
                cout << "Joypad Interrupt" << endl;

                if_update(I_JOYPAD, false);

                mem.set_pc(IV_JOYPAD);
            }
        }
    }

    if (ei_count)
    {
        process_ei_count();
    }
}

bool Interrupt::get_ime()
{
    return ime;
}

void Interrupt::disable_interrupts()
{
    ime = false;
}

void Interrupt::enable_interrupts()
{
    ei_count = 1;
}

//------------------------- Private ----------------------------------------

// Process ei_count
void Interrupt::process_ei_count()
{
    // Process the EI counter (used to delay EI instruction 1 cycle)
    // 0 = Normal State, 1 = Flag Set, 2 = Delayed 1 cycle ready to reset
    if (ei_count == 0)
    {
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

bool Interrupt::if_get(u8 int_flag)
{
    u8 flag_val;
    flag_val = mem.get_byte(A_IFLAGS) & int_flag;
    return !(flag_val==0);
}

void Interrupt::if_update(u8 int_flag, bool flg_val)
{
    if (flg_val)
    {
        i_flags |= int_flag;
    }
    else
    {
        i_flags &= ~int_flag;
    }

    mem.write_byte(A_IFLAGS,i_flags);
}
