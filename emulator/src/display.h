#pragma once

#include "displayconst.h"
#include "Memory.h"
#include "render.h"
#include "interrupt.h"
#include "lib/Error.h"

#include <string>
#include <iostream>
#include "lib/Types.h"

enum class DisplayMode {
  Mode0,
  Mode1,
  Mode2,
  Mode3
};

class GBDisplay
{
    public:
        GBDisplay(Memory& mem_in, Interrupt& ir_in);  
        void display_cycle(u32 cycles);
        void update_line();
        void draw_frame();
        bool new_frame_is_drawn();
        u32* get_buffer();
        virtual ~GBDisplay();

    private:
        bool cycles_reached(DisplayMode display_mode) const;
        void update_stat_reg(u8 mode_val);
        void colour();
        void clear_pixels();

        Render ren;
        Memory& mem;
        Interrupt& ir;

        ColourValue pixels[DISP_W*DISP_H];
        u32* pixels_coloured;
        u32 line_clock_cycles;
        bool new_frame_drawn;
};
