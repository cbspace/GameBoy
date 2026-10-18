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
        void notify_lcdc_write(u8 new_lcdc);
        void update_line();
        void draw_frame();
        bool new_frame_is_drawn();
        u32* get_buffer();
        virtual ~GBDisplay();

        i32 line_clock_cycles;
    private:
        void update_stat_reg(u8 mode_val);
        void colour();
        void clear_pixels();

        Render ren;
        Memory& mem;
        Interrupt& ir;

        ColourValue pixels[DISP_W*DISP_H];
        u32* pixels_coloured;
        bool new_frame_drawn;
        bool lcd_enabled = true;
        bool first_scanline_after_enable = false;
};
