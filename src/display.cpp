#include "display.h"

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

// logo_data is char* array read from the ROM (48 bytes long)
/*  Data Format:
    Gameboy tiles are 8x8 pixels in size. An 8x8 image is
    represented by 16 bytes. There are 4 'colours' that
    can be shown at each pixel: eg:
    ..1331..   ->       00111100
                        00011000
*/

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

// Render a single frame
void Display::render_frame()
{
    cout << "rendering\n";
}
