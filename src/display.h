#pragma once

#include "displayconst.h"
#include "Memory.h"
#include "render.h"
#include "interrupt.h"
#include "Clock.h"
#include "lib/Error.h"

#include <string>
#include <iostream>
#include "lib/Types.h"
//#include <SDL2/SDL.h>
#include <optional>

// GBDisplay Class looks after Window
// Render Class looks after pixel array

class GBDisplay
{
    public:
        GBDisplay(Memory& mem_in, Interrupt& ir_in, Clock& clk_in);  

        optional<Error> init();
        //void set_window_title(string title_add);
        void display_cycle();
        void update_line();
        void draw_frame();
        u32* get_buffer();
        virtual ~GBDisplay();

    private:
        void update_stat_reg(u8 mode_val);
        void colour();
        void scale();
        void clear_pixels();

        Render ren;
        Memory& mem;
        Interrupt& ir;
        Clock& clk;

        u32* display_buffer;
        ColourValue pixels[DISP_W*DISP_H];
        u32 pixels_coloured[DISP_W*DISP_H];

};
