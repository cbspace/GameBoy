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

// logo_data is char* array read from the ROM (48 bytes long)
/*  Data Format:
    Gameboy tiles are 8x8 pixels in size. An 8x8 image is
    represented by 16 bytes. There are 4 'colours' that
    can be shown at each pixel: eg:
    ..1331..   ->       00111100
                        00011000
*/

// Render a single frame
void Display::render_frame()
{
    cout << "rendering\n";

    /// Temp fill canvas
    for (uint16_t j = 0; j < height; j++)
    {
        for (uint16_t i = 0; i < width; i++)
        {
            pixels[j*width + i] = BG_COLOUR_ARGB;
        }
    }

    /// Draw logo
    uint8_t lg[6];
    lg[0] = mem->get_byte(0x8010);
    lg[1] = mem->get_byte(0x8012);
    lg[2] = mem->get_byte(0x8014);
    lg[3] = mem->get_byte(0x8016);
    lg[4] = mem->get_byte(0x8018);
    lg[5] = mem->get_byte(0x8020);

    for (uint16_t y = 0; y < 8; y++)
    {
        for (uint16_t x = 0; x < 8; x++)
        {
            if ((lg[y] | (1<<(7-x))) >> (7-x) == 1)
            {
                pixels[y*width + x] = 0x00000000;
            }
        }
    }
    /*

    // Draw background tiles
    for (uint8_t bg_tile_x = 0; bg_tile_x < BG_TILES_MAX; bg_tile_x++)
    {
            for (uint8_t bg_tile_y = 0; bg_tile_y < BG_TILES_MAX; bg_tile_y++)
            {

            }
    }*/

    // Update texture
    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
