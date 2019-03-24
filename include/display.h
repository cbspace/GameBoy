#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>

const int BG_COLOUR_R 0x92
const int BG_COLOUR_G 0xAD
const int BG_COLOUR_B 0x26

class display
{
        SDL_Window* window;
        SDL_Surface* drawSurface;
        SDL_Surface* logoImage; // Temp

    public:
        display(const char* title, const int width, const int height);
        virtual ~display();
};

#endif // DISPLAY_H
