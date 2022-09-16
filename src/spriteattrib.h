#pragma once

#include "lib/Types.h"

#define SPRITE_ATTRIB_FLAG_PRIORITY		7
#define SPRITE_ATTRIB_FLAG_Y_FLIP		6
#define SPRITE_ATTRIB_FLAG_X_FLIP		5
#define SPRITE_ATTRIB_FLAG_PALETTE		4

/*
Class to define sprite atrributes

Sprite data in OAM consistes of 4 bytes (32bit). Note that DMG only uses 28 bits;
	Byte 1 - Sptrite y position
    Byte 2 - Sptrite x position
    Byte 3 - Sptrite tile number
    Byte 4 - Sptrite flags:
    	Bit 7 - Priority: 0 = On top of BG and Window, 1 = hidden behind BG colours
    	Bit 6 - Y flip: 1 = Flip vertically
    	Bit 5 - X flip: 1 = Flip horizontally
    	Bit 4 - Paletter Number: 1 = Obj pal 1, 0 = Ohn pal 0
    	Bits 3-0 Not used
};

*/


class SpriteAttrib
{
    public:
		SpriteAttrib();
      	void set_sprite_x(u8 sprite_x);
      	void set_sprite_y(u8 sprite_y);
      	void set_sprite_tile_no(u8 tile_no_in);
      	void set_sprite_flags(u8 flag_data);
      	u8 get_x();
      	u8 get_y();
      	u8 get_tile_no();
      	u8 get_flag(u8 flag);

    private:
      	u8 x_pos;
      	u8 y_pos;
      	u8 tile_no;

      	u8 flag_priority;
      	u8 flag_x_flip;
      	u8 flag_y_flip;
      	u8 flag_palette;
};