#ifndef DISPLAY_H
#define DISPLAY_H

#include <string>
#include <SDL2/SDL.h>

#define SCALING_FACTOR 2        // Scale display size by this amount
#define WINDOW_TITLE "EmuBoy"   // Title to display on SDL Window

#define BG_COLOUR_R 0x92
#define BG_COLOUR_G 0xAD
#define BG_COLOUR_B 0x26

using namespace std;

static const int width = 160*SCALING_FACTOR;    // Screen width in pixels
static const int height = 144*SCALING_FACTOR;   // Screen height in pixels

class Display
{
    public:
        Display();
        int init();
        void set_title(string title_add);
        virtual ~Display();

        void draw_logo(char* logo_data);        // Draw statup logo!

    private:
        SDL_Window* window;
        SDL_Surface* drawSurface;
};

#endif // DISPLAY_H
