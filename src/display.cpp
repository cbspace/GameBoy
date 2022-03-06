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
    sdlRenderer = NULL;
    display_buffer = NULL;
    pixels = NULL;
    ren = NULL;
    mem = NULL;
    texture = NULL;
}

// Set pointer used to access memory object
void Display::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

// Set pointer used to access render object
void Display::attach_render(Render* ren_in)
{
    ren = ren_in;
}

// Set the window title
void Display::set_title(string title_add)
{
    string window_title_const(WINDOW_TITLE);
    string new_window_title = window_title_const + " - " + title_add;
    SDL_SetWindowTitle(window, new_window_title.c_str());
}

// Update the frame on the display
void Display::update_frame()
{
    cout << "rendering\n";

    // Render pixel map
    ren->render_frame();

    // Scale the pixels to match buffer
    scale();

    // Add colour data to buffer
    colour();

    // Update texture and display buffer
    SDL_UpdateTexture(texture, NULL, display_buffer, width * sizeof(uint32_t));
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
}

// Add colour data to buffer
void Display::colour()
{
	uint32_t current_pixel_ref;

    // Loop through buffer
    for (uint16_t j = 0; j < height; j++)
    {
        for (uint16_t i = 0; i < width; i++)
        {
        	current_pixel_ref = j*width + i;
        	switch(display_buffer[current_pixel_ref])
        	{
        	case 0x00:
        		display_buffer[current_pixel_ref] = COLOUR_BG_ARGB;
        		break;
        	case 0x01:
        		display_buffer[current_pixel_ref] = COLOUR_C1_ARGB;
        		break;
        	case 0x02:
        		display_buffer[current_pixel_ref] = COLOUR_C2_ARGB;
        		break;
        	case 0x03:
        		display_buffer[current_pixel_ref] = COLOUR_C3_ARGB;
        		break;
        	default:
        		display_buffer[current_pixel_ref] = 0xffffffff;
        		break;
        	}

        }
    }
}

// Scale the pixels to match buffer size
void Display::scale()
{
    // Loop through pixels
    for (uint16_t y = 0; y < DISP_H; y++)
    {
        for (uint16_t x = 0; x < DISP_W; x++)
        {
        	// Loop through scaled pixels
            for (uint16_t y1 = 0; y1 < SCALING_FACTOR; y1++)
            {
                for (uint16_t x1 = 0; x1 < SCALING_FACTOR; x1++)
                {
                	display_buffer[(y * SCALING_FACTOR + y1) * width + x * SCALING_FACTOR + x1] = pixels[y * DISP_W + x];
                }
            }
        }
    }
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
            // Create the renderer
        	sdlRenderer = SDL_CreateRenderer(window, -1, 0);

            // Create the texture
            texture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

            // Display buffer
            display_buffer = new uint32_t[width*height];

            // The pixel canvas
            pixels = new uint8_t[width*height];

            // Update pixels reference in renderer object
            ren->attach_pixels(pixels);

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
	SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);

	texture = NULL;
	sdlRenderer = NULL;
	window = NULL;

	// Free up dynamically allocated memory
	delete[] display_buffer;

	// Tidy up
	SDL_Quit();
}
