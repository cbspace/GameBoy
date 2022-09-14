#pragma once

#include "spriteattrib.h"
#include "memory.h"
#include "displayconst.h"

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
	    Render(Memory& mem_in, uint8_t (&pix_in)[width*height]);
        void render_line(uint8_t y);
        void refresh_sprites();
        virtual ~Render();

    private:
        void draw_bg_line(uint8_t line_y);
        void draw_sprites_line(uint8_t line_y);

        Memory& mem;
        uint8_t (&pixels)[width*height];
        SpriteAttrib spr_att[SPRITE_TILES_MAX];

        uint8_t get_bg_pixel(uint8_t bg_y, uint8_t bg_x, uint16_t bg_tdt, uint8_t bg_tm);
        uint8_t get_sprite_pixel(uint8_t tile_no, uint8_t sprite_y, uint8_t sprite_x);
};
