#pragma once

#include "spriteattrib.h"
#include "memory.h"
#include "displayconst.h"

#include <string>
#include "lib/Types.h"

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
	    Render(Memory& mem_in, u8 (&pix_in)[width*height]);
        void render_line(u8 y);
        void refresh_sprites();
        virtual ~Render();

    private:
        void draw_bg_line(u8 line_y);
        void draw_sprites_line(u8 line_y);

        Memory& mem;
        u8 (&pixels)[width*height];
        SpriteAttrib spr_att[SPRITE_TILES_MAX];

        u8 get_bg_pixel(u8 bg_y, u8 bg_x, u16 bg_tdt, u8 bg_tm);
        u8 get_sprite_pixel(u8 tile_no, u8 sprite_y, u8 sprite_x);
};
