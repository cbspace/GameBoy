#pragma once

#include "displayconst.h"
#include "../memory/memory.h"
#include "../render/render.h"
#include "../interrupt/interrupt.h"
#include "../clock/clock.h"
#include "../lib/Error.h"

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
        Display();        
        uint8_t* pixels;
        void attach_memory(Memory* mem_in);
        void attach_render(Render* ren_in);
        void attach_interrupt(Interrupt* interrupt_in);
        void attach_clock(Clock* clk_in);

        optional<Error> init();
        void set_title(string title_add);
        void display_cycle();
        void update_line();
        void draw_frame();
        virtual ~Display();

    private:
        Memory* mem;
        Render* ren;
        Interrupt* ir;
        Clock* clk;
        uint32_t* display_buffer;

        void update_stat_reg(uint8_t mode_val);
        void colour();
        void scale();
        void clear_pixels();

        SDL_Window* window;
        SDL_Surface* drawSurface;
        SDL_Renderer* sdlRenderer;
        SDL_Texture* texture;

};