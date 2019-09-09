#ifndef DISPLAY_H
#define DISPLAY_H

#include "memory.h"

#include <string>
#include <stdint.h>
#include <SDL2/SDL.h>

#define SCALING_FACTOR      2           // Scale display size by this amount
#define WINDOW_TITLE        "EmuBoy"    // Title to display on SDL Window

#define BG_COLOUR_R         0x92
#define BG_COLOUR_G         0xAD
#define BG_COLOUR_B         0x26

#define BG_COLOUR_ARGB      0xff92AD26

#define BG_TILES_MAX        32
//#define SPRITE_TILES_MAX    32
#define BG_PX_MAX           255
//#define SPRITE_PX_MAX       255

using namespace std;

static const int width = 160*SCALING_FACTOR;    // Screen width in pixels
static const int height = 144*SCALING_FACTOR;   // Screen height in pixels

class Display
{
    public:
        Display();                              // Constructor
        void attach_memory(Memory* mem_in);     // Set pointer used to access memory object
        int8_t init();                          // Set up SDL
        void set_title(string title_add);       // Set title of SDL window
        virtual ~Display();                     // Destructor

        // Rendering
        void render_frame();                    // Render a single frame

    private:
        Memory* mem;                            // Pointer to memory object

        SDL_Window* window;
        SDL_Surface* drawSurface;
        SDL_Renderer* renderer;
        SDL_Texture* texture;

        uint32_t* pixels;
        void draw_bg();                         // Draw bg tiles into pixel array
        void draw_tile(uint16_t addr);          // Draw an individual tile located at adddr
};

#endif // DISPLAY_H
