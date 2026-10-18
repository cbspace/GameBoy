#pragma once

#include "Memory.h"
#include "interrupt.h"
#include "lib/Types.h"

class Cpu
{

    public:
        Cpu(Memory& mem_in, Interrupt& ir_in, Timer& timer_in);
        void add_ticks(u8 extra_ticks);
        u32 cycle();

    private:
        void process_instruction(u8 rom_byte);
        void process_cb_instruction(u8 rom_byte);
        void halt();
        void stop();
        void cancel_stop();

        Memory& mem;
        Interrupt& ir;
        Timer& timer; //for debug
        u8 byte_in;
        u32 ticks;

        bool halt_flag;
        bool halt_bug_flag;
        bool stop_flag;
};