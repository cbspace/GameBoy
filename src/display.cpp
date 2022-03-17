#include "display.h"
#include "displayconst.h"
#include "memory.h"
#include "interrupt.h"
#include "clock.h"

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
    ir = NULL;
    clk = NULL;
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

// Set pointer used to access interrupt object
void Display::attach_interrupt(Interrupt* interrupt_in)
{
    ir = interrupt_in;
}

// Set pointer used to access clock object
void Display::attach_clock(Clock* clock_in)
{
    clk = clock_in;
}

// Set the window title
void Display::set_title(string title_add)
{
    string window_title_const(WINDOW_TITLE);
    if (title_add.length() > 0) {
    	string new_window_title = window_title_const + " - " + title_add;
    	SDL_SetWindowTitle(window, new_window_title.c_str());
    }
    else {
    	SDL_SetWindowTitle(window, window_title_const.c_str());
    }
}

// Draw frame to display
void Display::draw_frame()
{
    // Scale the pixels to match buffer
    scale();

    // Add colour data to buffer
    colour();

    // Update texture and display buffer
    SDL_UpdateTexture(texture, NULL, display_buffer, width * sizeof(uint32_t));
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);

    // Delay until next frame
    clk->frame_delay();
}

// Update the current line
void Display::update_line()
{
	uint8_t ly_val;

	// Get current LY value from register
    ly_val = mem->get_byte(R_LY);

    if (ly_val < DISP_H)
    {
        // Render line
        ren->render_line(ly_val);

        // Update LY register
        mem->write_byte(R_LY, ly_val + 1);
    }
    else if (ly_val == DISP_H)
    {
        // Draw the frame
    	draw_frame();

    	// Set VBLANK interrupt flag
        ir->if_update(I_VBLANK, true);

        // Update LY register
        mem->write_byte(R_LY, ly_val + 1);
    }
    else if (ly_val < LCD_Y_MAX)
    {
        // Update LY register
        mem->write_byte(R_LY, ly_val + 1);
    }
    else
    {
        // Clear VBLANK interrupt flag
        ir->if_update(I_VBLANK, false);

        // Reset LY register
        mem->write_byte(R_LY, 0x00);
    }
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
