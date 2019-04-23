#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>

#define scaling_factor 2        // Scale display size by this amount
#define window_title "EmuBoy"   // Title to display on SDL Window

#define BG_COLOUR_R 0x92
#define BG_COLOUR_G 0xAD
#define BG_COLOUR_B 0x26

static const int width = 160*scaling_factor;    // Screen width in pixels
static const int height = 144*scaling_factor;   // Screen height in pixels

class Display
{
    public:
        Display();
        int init();
        virtual ~Display();

    private:
        SDL_Window* window;
        SDL_Surface* drawSurface;
};

#endif // DISPLAY_H
