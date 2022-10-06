#pragma once

#include "displayconst.h"
#include "spriteattrib.h"
#include "lib/Types.h"
#include "Memory.h"
#include <iostream>
#include <string>

// GBDisplay Class looks after Window
// Render Class looks after pixel array

/* logo_data is char* array read from the ROM (48 bytes long)
    Data Format:
    Gameboy tiles are 8x8 pixels in size. An 8x8 image is
    represented by 16 bytes. There are 4 'colours' that
    can be shown at each pixel: eg:
    ..1331..   ->       00111100
                        00011000
*/

const u32 COLOUR_BG_RGB =   0x92AD26;       // Background colour
const u32 COLOUR_C1_RGB =   0x6D811C;       // Colour 1
const u32 COLOUR_C2_RGB =   0x485612;       // Colour 2
const u32 COLOUR_C3_RGB =   0x242B09;       // Colour 3

const u32 BG_TILES_MAX =         32;         // BG is 32 x 32 tiles
const u32 SPRITE_TILES_MAX =     40;         // Sprite OAM memory for 40 sprites

using namespace std;

class Render
{
    public:
        Render(Memory& mem_in, ColourValue (&pix_in)[DISP_W*DISP_H]);
        void render_line(u8 line_y);
        void refresh_sprites();
        virtual ~Render();

    private:
        void draw_bg_line(u8 line_y);
        void draw_sprites_line(u8 line_y);

        Memory& mem;
        ColourValue (&pixels)[DISP_W*DISP_H];
        SpriteAttrib spr_att[SPRITE_TILES_MAX];

        ColourValue get_bg_pixel(u8 bg_y, u8 bg_x, u16 bg_tdt, u8 bg_tm);
        ColourValue get_sprite_pixel(u8 tile_no, u8 sprite_y, u8 sprite_x);
};
