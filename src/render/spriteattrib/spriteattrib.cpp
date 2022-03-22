#include "spriteattrib.h"
#include <stdint.h>


// Constructor empty
SpriteAttrib::SpriteAttrib()
{
	x_pos = 0;
	y_pos = 0;
	tile_no = 0;
  	flag_priority = 0;
  	flag_x_flip = 0;
  	flag_y_flip = 0;
  	flag_palette = 0;
}

// Set x position
void SpriteAttrib::set_sprite_x(uint8_t sprite_x)
{
	x_pos = sprite_x;
}

// Set y position
void SpriteAttrib::set_sprite_y(uint8_t sprite_y)
{
	y_pos = sprite_y;
}

// Set tile number
void SpriteAttrib::set_sprite_tile_no(uint8_t tile_no_in)
{
	tile_no = tile_no_in;
}

// Set flag data
void SpriteAttrib::set_sprite_flags(uint8_t flag_data)
{
	uint8_t mask_val,flag_val;

	// Loop through flags and load data
	for (uint8_t i = SPRITE_ATTRIB_FLAG_PALETTE; i <= SPRITE_ATTRIB_FLAG_PRIORITY; i++)
	{
		mask_val = 1 << i;
		flag_val = (flag_data & mask_val) >> 1;

		switch (i)
		{
		case SPRITE_ATTRIB_FLAG_PALETTE:
			flag_palette = flag_val;
			break;
		case SPRITE_ATTRIB_FLAG_X_FLIP:
			flag_x_flip = flag_val;
			break;
		case SPRITE_ATTRIB_FLAG_Y_FLIP:
			flag_y_flip = flag_val;
			break;
		case SPRITE_ATTRIB_FLAG_PRIORITY:
			flag_priority = flag_val;
			break;
		}
	}
}

// Get x position
uint8_t SpriteAttrib::get_x()
{
	return x_pos;
}

// Get y position
uint8_t SpriteAttrib::get_y()
{
	return y_pos;
}

// Get tile number
uint8_t SpriteAttrib::get_tile_no()
{
	return tile_no;
}
