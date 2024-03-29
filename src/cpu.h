#pragma once

#include "Memory.h"
#include "Clock.h"
#include "interrupt.h"
#include "LibCB/Types.h"

class Cpu
{

    public:
        Cpu(Memory& mem_in, Clock& clk_in, Interrupt& ir_in);
        void cycle();

    private:
        void process_instruction(u8 rom_byte);
        void process_cb_instruction(u8 rom_byte);
        void halt();
        void stop();
        void cancel_stop();

        Memory& mem;
        Clock& clk;
        Interrupt& ir;
        u8 byte_in;

        bool halt_flag;
        bool stop_flag;
};