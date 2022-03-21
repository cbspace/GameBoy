#include "render.h"
#include "spriteattrib/spriteattrib.h"
#include "../display/displayconst.h"
#include "../memory/memory.h"

#include <iostream>
#include <stdint.h>
#include <string>

using namespace std;

/* Rendering Steps:
 * 1 - Draw Single Line to Buffer
 *  a) Draw Background and Window
 *  b) Draw Sprites
 * 2 - H-Blank period
 * 3 - After last line
 *  a) Perform scaling on Buffer
 *  b) Write Buffer to display
 *  c) V-Blank ISR
 */

// Constructor
Render::Render()
{
    // Initialise pointers
	mem = NULL;
	pixels = NULL;
	spr_att = new SpriteAttrib[SPRITE_TILES_MAX];
}

// Set pointer used to access memory object
void Render::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

// Set pointer to pixel array
void Render::attach_pixels(uint8_t* pix_in)
{
	pixels = pix_in;
}

// Render a single frame
void Render::render_line(uint8_t y)
{
//	if (mem->get_bit(R_LCDC, R_LCDC_BG_WIN_DISPLAY))
//	{
		// Draw the background tiles - single line
		//draw_bg_line(y);
//	}

//	if (mem->get_bit(R_LCDC, R_LCDC_SPRITE_DISPLAY))
//	{
		// Draw the sprites - single line
		draw_sprites_line(y+1);
//	}
}

// Refresh sprites
void Render::refresh_sprites()
{
	// OAM Data byte
	uint16_t oam_data_addr;

	for (uint8_t i = 0; i < SPRITE_TILES_MAX; i++)
	{
		oam_data_addr = A_OAM + i * OAM_DATA_LENGTH;
		// Set x
		spr_att[i].set_sprite_x(mem->get_byte(oam_data_addr));
		// Set y
		spr_att[i].set_sprite_y(mem->get_byte(oam_data_addr + 1));
		// Set tile number
		spr_att[i].set_sprite_tile_no(mem->get_byte(oam_data_addr + 2));
		// Set flags
		spr_att[i].set_sprite_flags(mem->get_byte(oam_data_addr + 3));
	}
}

// Draw sprite tiles into pixel array
void Render::draw_sprites_line(uint8_t line_y)
{
	SpriteAttrib *current_sprite_attrib;		// Current sprite attributes
//	uint8_t sprite_size;					// 0 = 8x8 mode, 1 = 8x16 mode (w x h)
	uint8_t sprite_x, sprite_y;				// Current sprite x and y value
	uint8_t x_start, x_finish;				// Overlapping x pixels to draw
	uint8_t pix_val, y_val;

//	sprite_size = mem->get_bit(R_LCDC, R_LCDC_SPRITE_SIZE);

    // Loop through sprites in OAM memory (Assuming 8x8 mode)
    for (uint8_t s = 0; s < SPRITE_TILES_MAX; s++)
    {
    	current_sprite_attrib = &spr_att[s];
    	sprite_y = current_sprite_attrib->get_y();

    	if (((sprite_y > 0) && (sprite_y < 16) && (line_y <= sprite_y)) ||
    		((sprite_y >= 16) && (sprite_y <= 144) && (line_y <= sprite_y) && (line_y >= sprite_y - 16)) ||
    		((sprite_y > 144) && (sprite_y < 160) && (line_y >= sprite_y - 16)))
    	{
    		y_val = 7 - (sprite_y - line_y);
    		sprite_x = current_sprite_attrib->get_x();

    		if ((sprite_x > 0) && (sprite_x <= 8))
    		{
    				x_start = 1;
    				x_finish = sprite_x;

    				for (uint8_t x = x_start; x < x_finish; x++)
    				{
    					pix_val = get_sprite_pixel(current_sprite_attrib->get_tile_no(),y_val,x);
    					pixels[line_y * DISP_W + x - 1] = pix_val;
    				}
    		}
    		else if ((sprite_x > 8) && (sprite_x <= 160))
    		{
    				x_start = sprite_x - 8;
    				x_finish = sprite_x;

    				for (uint8_t x = x_start; x < x_finish; x++)
    				{
    					pix_val = get_sprite_pixel(current_sprite_attrib->get_tile_no(),y_val,x);
    					pixels[line_y * DISP_W + x - 1] = pix_val;
    				}
    		}
    		else if ((sprite_x > 160) && (sprite_x <= 168))
    		{
    				x_start = sprite_x - 160;
    				x_finish = 160;

    				for (uint8_t x = x_start; x < x_finish; x++)
    				{
    					pix_val = get_sprite_pixel(current_sprite_attrib->get_tile_no(),y_val,x);
    					pixels[line_y * DISP_W + x - 1] = pix_val;
    				}
    		}
    	}
    }
}

// Get a sprite pixel from a tile
uint8_t Render::get_sprite_pixel(uint8_t tile_no, uint8_t sprite_y, uint8_t sprite_x)
{
	uint8_t byte_number, bit_number;
	uint8_t tile_byte1, tile_byte2;
	uint16_t tile_addr;

	// Calculate start address for tile
	tile_addr = A_TDT1 + tile_no * 16;

	byte_number = sprite_y * 2;
	bit_number = 7 - sprite_x;

	// Load data for tile byte
	tile_byte1 = mem->get_byte(tile_addr + byte_number);
	tile_byte2 = mem->get_byte(tile_addr + byte_number + 1);

	return ((tile_byte2 & (1<<(bit_number))) >> (bit_number) << 1)
		   + ((tile_byte1 & (1<<(bit_number))) >> (bit_number));
}

// Draw bg tiles into pixel array
void Render::draw_bg_line(uint8_t line_y)
{
    // BG Tile Data Table Address
	uint16_t bg_tdt;

	// BG Tile Map Number
	uint8_t bg_tm;

	// Scroll X and Y value
    uint8_t scroll_x, scroll_y;

    // X and Y position of pixel on screen
    uint8_t pix_x, pix_y;

    // Get BG Tile Map Number (0=BGTM1, 1=BGTM2)
    bg_tm = mem->get_bit(R_LCDC, 3);

    // Get BG Tile Data Table Address (TDT1 or TDT2)
    bg_tdt = (mem->get_bit(R_LCDC, 4) == 1) ? A_TDT1 : A_TDT2;

    // Get value of Scroll X and Y registers
    scroll_x = mem->get_byte(R_SCX);
    scroll_y = mem->get_byte(R_SCY);

    // Set Y value of pixel
    pix_y = scroll_y + line_y;

    // Loop through pixels in line
    for (uint8_t x = 0; x < DISP_W; x++)
    {
    	pix_x = (scroll_x + x) % DISP_W;
    	pixels[line_y * DISP_W + x] = get_bg_pixel(pix_y, pix_x, bg_tdt, bg_tm);
    }
}

// Get a bg pixel from a tile
uint8_t Render::get_bg_pixel(uint8_t bg_y, uint8_t bg_x, uint16_t bg_tdt, uint8_t bg_tm)
{
	uint8_t tile_number, tile_pos_x, tile_pos_y;
	uint8_t byte_number, bit_number;
	uint8_t tile_byte1, tile_byte2;
	uint16_t tile_addr;

	int8_t tile_number_signed;
	int32_t tile_addr_signed;

	//Calculate tile position
	tile_pos_y = bg_y / 8;
	tile_pos_x = bg_x / 8;

	if (bg_tm == 0) {
		// Read tile number from bg tile map
		tile_number = mem->get_byte(A_BGTM1 + tile_pos_y*32 + tile_pos_x);

		// Calculate start address for tile
		tile_addr = bg_tdt + tile_number * 16;

		byte_number = (bg_y - tile_pos_y * 8) * 2;
		bit_number = 7 - (bg_x - tile_pos_x * 8);

		// Load data for tile byte
		tile_byte1 = mem->get_byte(tile_addr + byte_number);
		tile_byte2 = mem->get_byte(tile_addr + byte_number + 1);
	}
	else {
		// Read tile number from bg tile map
		tile_number_signed = mem->get_byte(A_BGTM2 + tile_pos_y*32 + tile_pos_x);

		// Calculate start address for tile
		tile_addr_signed = (int32_t)bg_tdt + (int32_t)tile_number_signed * 16;

		byte_number = (bg_y - tile_pos_y * 8) * 2;
		bit_number = 7 - (bg_x - tile_pos_x * 8);

		// Load data for tile byte
		tile_byte1 = mem->get_byte((uint16_t)tile_addr_signed + (uint16_t)byte_number);
		tile_byte2 = mem->get_byte((uint16_t)tile_addr_signed + (uint16_t)byte_number + 1);
	}

	return ((tile_byte2 & (1<<(bit_number))) >> (bit_number) << 1)
		   + ((tile_byte1 & (1<<(bit_number))) >> (bit_number));
}

// Destructor
Render::~Render()
{


}
