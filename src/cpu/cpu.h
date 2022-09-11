#pragma once

#include "../memory/memory.h"
#include "../clock/clock.h"
#include "../interrupt/interrupt.h"
#include "../emudebug/emudebug.h"
#include <stdint.h>

class Cpu
{

    public:
        Cpu();
        void attach_memory(Memory* mem_in);
        void attach_clock(Clock* clock_in);
        void attach_interrupt(Interrupt* interrupt_in);
        void attach_emudebug(Emudebug* debug_in);
        void cycle();

    private:
        void process_instruction(uint8_t rom_byte);
        void process_cb_instruction(uint8_t rom_byte);

        Memory* mem;
        Clock* clk;
        Interrupt* ir;
        Emudebug* edb;
        uint8_t byte_in;
};