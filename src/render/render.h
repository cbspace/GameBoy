#pragma once

#include "spriteattrib/spriteattrib.h"
#include "../memory/memory.h"
#include "../display/displayconst.h"

#include <string>
#include <stdint.h>

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
	    uint8_t* pixels;					   		// Array of pixels to display

        Render();                              		// Constructor
        void attach_memory(Memory* mem_in);    		// Set pointer used to access memory object
        void attach_pixels(uint8_t* pix_in);   		// Set pointer to pixel array
        void render_line(uint8_t y);           		// Render a single line
        void refresh_sprites();						// Refresh sprites
        virtual ~Render();                     		// Destructor

    private:
        Memory* mem;                            	// Pointer to memory object
        SpriteAttrib *spr_att;	    				// Array of sprite attributes

        void draw_bg_line(uint8_t line_y);      	// Draw bg tiles into pixel array
        void draw_sprites_line(uint8_t line_y); 	// Draw sprite tiles into pixel array
        // Get a bg pixel from a tile
        uint8_t get_bg_pixel(uint8_t bg_y, uint8_t bg_x, uint16_t bg_tdt, uint8_t bg_tm);
        // Get a bg sprite from a tile
        uint8_t get_sprite_pixel(uint8_t tile_no, uint8_t sprite_y, uint8_t sprite_x);
};