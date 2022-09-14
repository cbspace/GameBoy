#pragma once

#include "displayconst.h"
#include "memory.h"
#include "render.h"
#include "interrupt.h"
#include "clock.h"
#include "lib/Error.h"

#include <string>
#include <stdint.h>
#include <SDL2/SDL.h>
#include <optional>

// Display Class looks after SDL Window
// Render Class looks after pixel array

enum class ColourValues {
    C0,
    C1,
    C2,
    C3
};

class Display
{
    public:
        Display(Memory& mem_in, Render& ren_in, Interrupt& ir_in, Clock& clk_in);  

        optional<Error> init();
        void set_title(string title_add);
        void display_cycle();
        void update_line();
        void draw_frame();
        virtual ~Display();

    private:
        void update_stat_reg(uint8_t mode_val);
        void colour();
        void scale();
        void clear_pixels();

        Memory& mem;
        Render& ren;
        Interrupt& ir;
        Clock& clk;

        uint32_t display_buffer[width*height];
        uint8_t pixels[width*height];

        SDL_Window* window;
        SDL_Surface* drawSurface;
        SDL_Renderer* sdlRenderer;
        SDL_Texture* texture;

};
