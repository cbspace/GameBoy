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

#define DISP_W   256           // Gameboy display width (Usually 160 pixels)
#define DISP_H   256           // Gameboy display height (Usually 144 pixels)

#define BG_TILES_MAX        32
//#define SPRITE_TILES_MAX    32
#define BG_PX_MAX           255
//#define SPRITE_PX_MAX       255

// Memory Locations - Video
#define A_TDT1      0x8000      // Tile Data Table 1 start address
#define A_TDT2      0x8800      // Tile Data Table 2 start address
#define A_BGTM      0x9800      // Background Tile Map start address

// Special Registers - Video
#define R_LCDC      0xff40      // LCD Control
#define R_LCDSTAT   0xff41      // LCD Status
#define R_SCY       0xff42      // Scroll Y
#define R_SCX       0xff43      // Scroll X
#define R_LY        0xff44      // LCD Y coordinate
#define R_LYC       0xff45      // LY Compare
#define R_DMA       0xff46      // Direct Memory Access
#define R_BGP       0xff47      // Background Palette
#define R_OBP0      0xff48      // Object Palette 0
#define R_OBP1      0xff49      // Object Palette 1
#define R_WY        0xff4a      // Window Y Position
#define R_WX        0xff4b      // Window X Position

using namespace std;

static const int width = DISP_W*SCALING_FACTOR;    // Screen width in pixels
static const int height = DISP_H*SCALING_FACTOR;   // Screen height in pixels

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
