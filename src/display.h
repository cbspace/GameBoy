#pragma once

#include "displayconst.h"
#include "Memory.h"
#include "render.h"
#include "interrupt.h"
#include "Clock.h"
#include "LibCB/Error.h"

#include <string>
#include <iostream>
#include "LibCB/Types.h"

class GBDisplay
{
    public:
        GBDisplay(Memory& mem_in, Interrupt& ir_in, Clock& clk_in);  
        void display_cycle();
        void update_line();
        void draw_frame();
        bool new_frame_is_drawn();
        u32* get_buffer();
        virtual ~GBDisplay();

    private:
        void update_stat_reg(u8 mode_val);
        void colour();
        void clear_pixels();

        Render ren;
        Memory& mem;
        Interrupt& ir;
        Clock& clk;

        ColourValue pixels[DISP_W*DISP_H];
        u32* pixels_coloured;
        bool new_frame_drawn;
};
