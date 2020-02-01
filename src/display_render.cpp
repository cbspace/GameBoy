#include "display.h"
#include "memory.h"

#include <iostream>
#include <stdint.h>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

// logo_data is char* array read from the ROM (48 bytes long)
/*  Data Format:
    Gameboy tiles are 8x8 pixels in size. An 8x8 image is
    represented by 16 bytes. There are 4 'colours' that
    can be shown at each pixel: eg:
    ..1331..   ->       00111100
                        00011000
*/

// Render a single frame
void Display::render_frame()
{
    cout << "rendering\n";

    // Fill canvas with BG Colour
    for (uint16_t j = 0; j < height; j++)
    {
        for (uint16_t i = 0; i < width; i++)
        {
            pixels[j*width + i] = BG_COLOUR_ARGB;
        }
    }

    // Draw the background tiles
    draw_bg();

    /// Reset LY register
    //mem->write_byte(R_LY,0);

    // Update texture
    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// Draw bg tiles into pixel array
void Display::draw_bg()
{
    // Byte array for a tile
    uint8_t tile[8];

    // Tile number of current tile (from bg tile map)
    uint8_t tile_number;

    // Memory address of tile
    // bg tile map located at 9800 to 9BFF
    uint16_t addr;

    // X and Y position of pixel on screen
    uint8_t pix_x, pix_y;

    // Scroll Y value
    uint8_t scroll_y;

    // Absolut Y value
    uint8_t abs_y;

    // Get value of SCROLLY register
    scroll_y = mem->get_byte(R_SCY);

    // Draw background tiles, loop through bg tile map
    for (uint8_t bg_tile_y = 0; bg_tile_y < BG_TILES_MAX; bg_tile_y++)
    {
        for (uint8_t bg_tile_x = 0; bg_tile_x < BG_TILES_MAX; bg_tile_x++)
        {
            // Read tile number from bg tile map
            tile_number = mem->get_byte(A_BGTM + bg_tile_y*32 + bg_tile_x);

            // Calculate start address for tile
            addr = A_TDT1 + tile_number * 16;

            // Load data for tile
            for (int b = 0; b < 8; b++)
            {
                tile[b] = mem->get_byte(addr + b * 2);
            }

            // Temp only draw viewable window
            if (bg_tile_x < 20)
            {
                // Draw a tile
                for (uint16_t y = 0; y < 8; y++)
                {
                    pix_y = bg_tile_y * 8 + y;

                    if (pix_y >= scroll_y)
                    {
                        abs_y = pix_y - scroll_y;

                        if (abs_y < 143)
                        {
                            for (uint16_t x = 0; x < 8; x++)
                            {
                                if ((tile[y] & (1<<(7-x))) >> (7-x) == 1)
                                {
                                    pix_x = bg_tile_x * 8 + x;
                                    pixels[abs_y * SCALING_FACTOR * SCALING_FACTOR * DISP_W + pix_x * SCALING_FACTOR] = 0x00000000;
                                    pixels[abs_y * SCALING_FACTOR * SCALING_FACTOR * DISP_W + pix_x * SCALING_FACTOR + 1] = 0x00000000;
                                    pixels[(abs_y * SCALING_FACTOR + 1) * SCALING_FACTOR * DISP_W + pix_x * SCALING_FACTOR] = 0x00000000;
                                    pixels[(abs_y * SCALING_FACTOR + 1) * SCALING_FACTOR * DISP_W + pix_x * SCALING_FACTOR + 1] = 0x00000000;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// Draw an individual tile located at adddr
void draw_tile(uint16_t addr)
{

}
