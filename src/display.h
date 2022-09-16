#pragma once

#include "displayconst.h"
#include "memory.h"
#include "render.h"
#include "interrupt.h"
#include "clock.h"
#include "lib/Error.h"

#include <string>
#include "lib/Types.h"
#include <SDL2/SDL.h>
#include <optional>

// Display Class looks after SDL Window
// Render Class looks after pixel array

class Display
{
    public:
        Display(Memory& mem_in, Interrupt& ir_in, Clock& clk_in);  

        optional<Error> init();
        void set_title(string title_add);
        void display_cycle();
        void update_line();
        void draw_frame();
        virtual ~Display();

    private:
        void update_stat_reg(u8 mode_val);
        void colour();
        void scale();
        void clear_pixels();

        Render ren;
        Memory& mem;
        Interrupt& ir;
        Clock& clk;

        u32 display_buffer[width*height];
        ColourValue pixels[DISP_W*DISP_H];
        u32 pixels_coloured[DISP_W*DISP_H];

        SDL_Window* window;
        SDL_Surface* drawSurface;
        SDL_Renderer* sdlRenderer;
        SDL_Texture* texture;

};
