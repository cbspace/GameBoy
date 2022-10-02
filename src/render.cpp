#include "render.h"

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

Render::Render(Memory& mem_in, ColourValue (&pix_in)[DISP_W*DISP_H]) :
	mem(mem_in),
	pixels(pix_in)
{}

void Render::render_line(u8 line_y)
{
//	if (mem.get_bit(R_LCDC, u8(LcdcReg::BG_WIN_DISPLAY)))
//	{
		draw_bg_line(line_y);
//	}

//	if (mem.get_bit(R_LCDC, u8(LcdcReg::SPRITE_DISPLAY)))
//	{
		//draw_sprites_line(y);
//	}
}

void Render::refresh_sprites()
{
	u16 oam_data_addr;

	for (u8 i = 0; i < SPRITE_TILES_MAX; i++)
	{
		oam_data_addr = A_OAM + i * OAM_DATA_LENGTH;
		// Set y
		spr_att[i].set_sprite_y(mem.get_byte(oam_data_addr));
		// Set x
		spr_att[i].set_sprite_x(mem.get_byte(oam_data_addr + 1));
		// Set tile number
		spr_att[i].set_sprite_tile_no(mem.get_byte(oam_data_addr + 2));
		// Set flags
		spr_att[i].set_sprite_flags(mem.get_byte(oam_data_addr + 3));
	}
}

void Render::draw_sprites_line(u8 line_y)
{
	SpriteAttrib current_sprite_attrib;		// Current sprite attributes
//	u8 sprite_size;					// 0 = 8x8 mode, 1 = 8x16 mode (w x h)
	u8 sprite_x, sprite_y;				// Current sprite x and y value
	u8 x_start, x_finish;				// Overlapping x pixels to draw
	u8 y_val, x_len;
	ColourValue pix_val;

//	sprite_size = mem.get_bit(R_LCDC, u8(LcdcReg::SPRITE_SIZE));

    // Loop through sprites in OAM memory (Assuming 8x8 mode)
    for (u8 s = 0; s < SPRITE_TILES_MAX; s++)
    {
    	current_sprite_attrib = spr_att[s];
    	sprite_y = current_sprite_attrib.get_y();

    	if (((sprite_y > 0) && (sprite_y < 16) && (line_y <= sprite_y)) ||
    		((sprite_y >= 16) && (sprite_y <= 144) && (line_y <= sprite_y) && (line_y >= sprite_y - 16)) ||
    		((sprite_y > 144) && (sprite_y < 160) && (line_y >= sprite_y - 16)))
    	{
    		y_val = 8 - (sprite_y - line_y);
    		sprite_x = current_sprite_attrib.get_x();

    		if ((sprite_x > 0) && (sprite_x <= 7))
    		{

    				for (u8 x = 0; x < sprite_x; x++)
    				{
    					pix_val = get_sprite_pixel(current_sprite_attrib.get_tile_no(),y_val,x+(8-sprite_x));
    					pixels[line_y * DISP_W + x] = pix_val;
    				}
    		}
    		else if ((sprite_x >= 8) && (sprite_x <= 160))
    		{
    				x_start = sprite_x - 8;
    				x_finish = sprite_x;
    				x_len = 8;

    				for (u8 x = 0; x < x_len; x++)
    				{
    					pix_val = get_sprite_pixel(current_sprite_attrib.get_tile_no(),y_val,x);
    					pixels[line_y * DISP_W + x_start + x] = pix_val;
    				}
    		}
    		else if ((sprite_x > 160) && (sprite_x <= 168))
    		{
    				x_start = sprite_x - 8;
    				x_finish = 160;
    				x_len = x_finish - x_start;

    				for (u8 x = 0; x < x_len; x++)
    				{
    					pix_val = get_sprite_pixel(current_sprite_attrib.get_tile_no(),y_val,x);
    					pixels[line_y * DISP_W + x_start + x] = pix_val;
    				}
    		}
    	}
    }
}

ColourValue Render::get_sprite_pixel(u8 tile_no, u8 sprite_y, u8 sprite_x)
{
	// Calculate start address for tile
	u16 tile_addr = A_TDT1 + tile_no * 16;

	u8 byte_number = sprite_y * 2;
	u8 bit_number = 7 - sprite_x;

	// Load data for tile byte
	u8 tile_byte1 = mem.get_byte(tile_addr + byte_number);
	u8 tile_byte2 = mem.get_byte(tile_addr + byte_number + 1);

	return static_cast<ColourValue>(((tile_byte2 & (1<<(bit_number))) >> (bit_number) << 1)
		   + ((tile_byte1 & (1<<(bit_number))) >> (bit_number)));
}

void Render::draw_bg_line(u8 line_y)
{
    // Get BG Tile Map Number (0=BGTM1, 1=BGTM2)
    u8 bg_tm = mem.get_bit(R_LCDC, 3);

    // Get BG Tile Data Table Address (TDT1 or TDT2)
    u16 bg_tdt = (mem.get_bit(R_LCDC, 4) == 1) ? A_TDT1 : A_TDT2;

    // Get value of Scroll X and Y registers
    u8 scroll_x = mem.get_byte(R_SCX);
    u8 scroll_y = mem.get_byte(R_SCY);

    // Set Y value of pixel
    u8 pix_y = scroll_y + line_y;

    // Loop through pixels in line
    for (u8 x = 0; x < DISP_W; x++)
    {
    	u8 pix_x = (scroll_x + x) % DISP_W;
    	pixels[line_y * DISP_W + x] = get_bg_pixel(pix_y, pix_x, bg_tdt, bg_tm);
    }
}

// Get a bg pixel from a tile
ColourValue Render::get_bg_pixel(u8 bg_y, u8 bg_x, u16 bg_tdt, u8 bg_tm)
{
	u8 bit_number;
	u8 tile_byte1, tile_byte2;

	//Calculate tile position
	u8 tile_pos_y = bg_y / 8;
	u8 tile_pos_x = bg_x / 8;

	if (bg_tm == 0) {
		// Read tile number from bg tile map
		u8 tile_number = mem.get_byte(A_BGTM1 + tile_pos_y*32 + tile_pos_x);

		// Calculate start address for tile
		u16 tile_addr = bg_tdt + tile_number * 16;

		u8 byte_number = (bg_y - tile_pos_y * 8) * 2;
		bit_number = 7 - (bg_x - tile_pos_x * 8);

		// Load data for tile byte
		tile_byte1 = mem.get_byte(tile_addr + byte_number);
		tile_byte2 = mem.get_byte(tile_addr + byte_number + 1);
	}
	else {
		// Read tile number from bg tile map
		i8 tile_number_signed = mem.get_byte(A_BGTM2 + tile_pos_y*32 + tile_pos_x);

		// Calculate start address for tile
		i32 tile_addr_signed = (i32)bg_tdt + (i32)tile_number_signed * 16;

		u8 byte_number = (bg_y - tile_pos_y * 8) * 2;
		bit_number = 7 - (bg_x - tile_pos_x * 8);

		// Load data for tile byte
		tile_byte1 = mem.get_byte((u16)tile_addr_signed + (u16)byte_number);
		tile_byte2 = mem.get_byte((u16)tile_addr_signed + (u16)byte_number + 1);
	}

	return static_cast<ColourValue>(((tile_byte2 & (1<<(bit_number))) >> (bit_number) << 1)
		   + ((tile_byte1 & (1<<(bit_number))) >> (bit_number)));
}

Render::~Render() {}
