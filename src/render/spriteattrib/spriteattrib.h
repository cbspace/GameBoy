#pragma once

#include <stdint.h>

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
      	void set_sprite_x(uint8_t sprite_x);			// Set x position
      	void set_sprite_y(uint8_t sprite_y);			// Set y position
      	void set_sprite_tile_no(uint8_t tile_no_in);	// Set tile number
      	void set_sprite_flags(uint8_t flag_data);		// Set flag data
      	uint8_t get_x();								// Get x position
      	uint8_t get_y();                    			// Get y position
      	uint8_t get_tile_no();              			// Get tile number
      	uint8_t get_flag(uint8_t flag);     			// Get flag

    private:
      	uint8_t x_pos;				// Sptrite x position
      	uint8_t y_pos;				// Sptrite y position
      	uint8_t tile_no;			// Sptrite tile number

      	uint8_t flag_priority;		// Sptrite flag priority
      	uint8_t flag_x_flip;		// Sptrite flag x flip
      	uint8_t flag_y_flip;		// Sptrite flag y flip
      	uint8_t flag_palette;		// Sptrite flag palette
};