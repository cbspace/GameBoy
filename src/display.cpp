#include "display.h"

// Initialise SDL and create a window
display::display(const char* title, const int width, const int height)
{
    // Initialise pointers
    window = NULL;
    drawSurface = NULL;

    // Initialise SDL
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        printf( "SDL unable to initialise! SDL_Error: %s\n", SDL_GetError() );
    }
    else
    {
        // Create a new SDL Window
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);

        if (window == NULL)
        {
            printf("Unable to create SDL Window! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            // Get thew window surface
            drawSurface = SDL_GetWindowSurface( window );

            // Fill the surface white
            SDL_FillRect( drawSurface, NULL, SDL_MapRGB ( drawSurface->format, BG_COLOUR_R, BG_COLOUR_G, BG_COLOUR_B ) );

            // Update the surface
            SDL_UpdateWindowSurface ( window );
        }
    }
}

// Close the SDL window and close SDL
display::~display()
{
    // Destroy the window and deallocate reference
	SDL_DestroyWindow(window);
	window = NULL;

	// Tidy up
	SDL_Quit();
}
