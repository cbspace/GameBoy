#ifndef DISPLAY_H
#define DISPLAY_H

#include "memory.h"
#include "render.h"

#include <string>
#include <stdint.h>
#include <SDL2/SDL.h>

#define SCALING_FACTOR      3                  // Scale display size by this amount
#define WINDOW_TITLE        "EmuBoy Render"    // Title to display on SDL Window

#define COLOUR_BG_ARGB      0xFF92AD26         // Background colour
#define COLOUR_C1_ARGB      0xFF6D811C         // Colour 1
#define COLOUR_C2_ARGB      0xFF485612         // Colour 2
#define COLOUR_C3_ARGB      0xFF242B09         // Colour 3

#define DISP_W   160           // Gameboy display width (Usually 160 pixels)
#define DISP_H   144           // Gameboy display height (Usually 144 pixels)

using namespace std;

static const int width = DISP_W * SCALING_FACTOR;    // Screen width in pixels
static const int height = DISP_H * SCALING_FACTOR;   // Screen height in pixels

// Display Class looks after SDL Window
// Render Class looks after pixel array

class Display
{
    public:
        Display();                              // Constructor
        uint8_t* pixels;						// Array of pixels to display
        void attach_memory(Memory* mem_in);     // Set pointer used to access memory object
        void attach_render(Render* ren_in);     // Set pointer used to access render object
        int8_t init();                          // Set up SDL
        void set_title(string title_add);       // Set title of SDL window
        void update_frame();                    // Update the frame on the display
        virtual ~Display();                     // Destructor

    private:
        Memory* mem;                            // Pointer to memory object
        Render* ren;                            // Pointer to renderer object

        uint32_t* display_buffer;				// Array of pixels for buffer
        void colour();							// Add colour data to pixel
        void scale();							// Scale the pixels to match buffer size

        // SDl Objects
        SDL_Window* window;
        SDL_Surface* drawSurface;
        SDL_Renderer* sdlRenderer;
        SDL_Texture* texture;

};

#endif // DISPLAY_H
