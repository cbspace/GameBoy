#ifndef RENDER_H
#define RENDER_H

#include "memory.h"
#include "display.h"

#include <string>
#include <stdint.h>
//#include <SDL2/SDL.h>

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

// Display Class looks after SDL Window
// Render Class looks after pixel array

/* logo_data is char* array read from the ROM (48 bytes long)
    Data Format:
    Gameboy tiles are 8x8 pixels in size. An 8x8 image is
    represented by 16 bytes. There are 4 'colours' that
    can be shown at each pixel: eg:
    ..1331..   ->       00111100
                        00011000
*/

using namespace std;

class Render
{
    public:
	    uint8_t* pixels;					   // Array of pixels to display

        Render();                              // Constructor
        void attach_memory(Memory* mem_in);    // Set pointer used to access memory object
        void attach_pixels(uint8_t* pix_in);   // Set pointer to pixel array
        virtual ~Render();                     // Destructor

        // Rendering
        void render_frame();                    // Render a single frame

    private:
        Memory* mem;                            // Pointer to memory object

        void draw_bg_line(uint8_t line_y);      // Draw bg tiles into pixel array
        uint8_t get_bg_pixel(uint8_t bg_y, uint8_t bg_x);  // Get a bg pixel from a tile
};

#endif // RENDER_H
