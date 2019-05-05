#include "display.h"

#include <iostream>
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

// Let's see if we can draw the famous Nintendo logo!
// logo_data is char* array read from the ROM (48 bytes long)
void Display::draw_logo(char* logo_data)
{
    for (int i = 0 ; i < 48; i++)
    {

    }
}

// Set the window title
void Display::set_title(string title_add)
{
    string window_title_const(WINDOW_TITLE);
    string new_window_title = window_title_const + " - " + title_add;
    SDL_SetWindowTitle(window, new_window_title.c_str());
}

// Initialise SDL and create a window
int Display::init()
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
            // Get thew window surface
            drawSurface = SDL_GetWindowSurface( window );

            // Fill the surface background colour
            SDL_FillRect( drawSurface, NULL, SDL_MapRGB ( drawSurface->format, BG_COLOUR_R, BG_COLOUR_G, BG_COLOUR_B ) );

            // Update the surface
            SDL_UpdateWindowSurface ( window );

            // Success
            return 0;
        }
    }
    return -1;
}

// Close the SDL window and close SDL
Display::~Display()
{
    // Destroy the window and deallocate reference
	SDL_DestroyWindow(window);
	window = NULL;

	// Tidy up
	SDL_Quit();
}
