#include "display.h"
#include "memory.h"

#include <iostream>
#include <stdint.h>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

// Constructor
Display::Display()
{
    // Initialise pointers
    window = NULL;
    drawSurface = NULL;
}

// Set pointer used to access memory object
void Display::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

// Set the window title
void Display::set_title(string title_add)
{
    string window_title_const(WINDOW_TITLE);
    string new_window_title = window_title_const + " - " + title_add;
    SDL_SetWindowTitle(window, new_window_title.c_str());
}

// Initialise SDL and create a window
int8_t Display::init()
{
    // Initialise SDL
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        cout << "SDL unable to initialise! SDL_Error: " << SDL_GetError() << endl;
    }
    else
    {
        // Create a new SDL Window
        window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

        if (window == NULL)
        {
            cout << "Unable to create SDL Window! SDL_Error: " << SDL_GetError() << endl;
        }
        else
        {
            // Get the window surface
            //drawSurface = SDL_GetWindowSurface(window);

            // Fill the surface background colour
            //SDL_FillRect(drawSurface, NULL, SDL_MapRGB(drawSurface->format, BG_COLOUR_R, BG_COLOUR_G, BG_COLOUR_B));

            // Create the renderer
            renderer = SDL_CreateRenderer(window, -1, 0);

            // Create the texture
            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

            // Update the surface
            //SDL_UpdateWindowSurface(window);

            // The pixel canvas
            pixels = new uint32_t[width*height];

            // Success
            return 0;
        }
    }
    return -1;
}

// Close the SDL window and close SDL
Display::~Display()
{
    // Destroy the window, texture and renderer and deallocate references
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

	texture = NULL;
	renderer = NULL;
	window = NULL;

	// Free up dynamically allocated memory
	delete[] pixels;

	// Tidy up
	SDL_Quit();
}
