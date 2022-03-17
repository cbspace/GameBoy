#include "displayconst.h"
#include "memory.h"
#include "render.h"

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
    // Draw the background tiles - single line
    draw_bg_line(y);

    // Draw the sprites - single line
    //draw_sprites_line(y);

}

// Draw bg tiles into pixel array
void Render::draw_bg_line(uint8_t line_y)
{
    // Scroll X and Y value
    uint8_t scroll_x, scroll_y;

    // X and Y position of pixel on screen
    uint8_t pix_x, pix_y;

    // Get value of Scroll X and Y registers
    scroll_x = mem->get_byte(R_SCX);
    scroll_y = mem->get_byte(R_SCY);

    // Set Y value of pixel
    pix_y = scroll_y + line_y;

    // Loop through pixels in line
    for (uint8_t x = 0; x < DISP_W; x++)
    {
    	pix_x = (scroll_x + x) % DISP_W;
    	pixels[line_y * DISP_W + x] = get_bg_pixel(pix_y, pix_x);
    }
}

// Get a bg pixel from a tile (TDT1)
uint8_t Render::get_bg_pixel(uint8_t bg_y, uint8_t bg_x)
{
	uint8_t tile_number, tile_pos_x, tile_pos_y;
	uint8_t byte_number, bit_number;
	uint8_t tile_byte1, tile_byte2;
	uint16_t tile_addr;

	//Calculate tile position
	tile_pos_y = bg_y / 8;
	tile_pos_x = bg_x / 8;

	// Read tile number from bg tile map
	tile_number = mem->get_byte(A_BGTM + tile_pos_y*32 + tile_pos_x);

	// Calculate start address for tile
	// bg tile map located at 9800 to 9BFF (TDT1)
	tile_addr = A_TDT1 + tile_number * 16;

	byte_number = (bg_y - tile_pos_y * 8) * 2;
	bit_number = 7 - (bg_x - tile_pos_x * 8);

	// Load data for tile byte
	tile_byte1 = mem->get_byte(tile_addr + byte_number);
	tile_byte2 = mem->get_byte(tile_addr + byte_number + 1);

	return ((tile_byte2 & (1<<(bit_number))) >> (bit_number) << 1)
		   + ((tile_byte1 & (1<<(bit_number))) >> (bit_number));
}

// Destructor
Render::~Render()
{


}
