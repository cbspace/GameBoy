#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

#define BG_COLOUR_R 0x92
#define BG_COLOUR_G 0xAD
#define BG_COLOUR_B 0x26

class display
{
        SDL_Window* window;
        SDL_Surface* drawSurface;

    public:
        display(const char* title, const int width, const int height);
        virtual ~display();
};

#endif // DISPLAY_H
