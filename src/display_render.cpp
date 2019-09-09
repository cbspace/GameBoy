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

    // Current memory address
    uint16_t addr;

    // Draw background tiles
    for (uint8_t bg_tile_x = 0; bg_tile_x < BG_TILES_MAX; bg_tile_x++)
    {
        for (uint8_t bg_tile_y = 0; bg_tile_y < BG_TILES_MAX; bg_tile_y++)
        {
            // Calculate start address for tile
            addr = 0x8000 + (bg_tile_y * 512) + bg_tile_x * 8;

            // Load data for tile
            for (int b = 0; b < 8; b++)
            {
                tile[b] = mem->get_byte(addr + b * 2);
            }

            // Temp only draw viewable window
            if ((bg_tile_x < 20) && (bg_tile_y < 18))
            {
                // Draw a tile
                for (uint16_t y = 0; y < 8; y++)
                {
                    for (uint16_t x = 0; x < 8; x++)
                    {
                        if ((tile[y] & (1<<(7-x))) >> (7-x) == 1)
                        {
                            pixels[(((bg_tile_y * 8 + y) * 2) * 160 + (bg_tile_x * 8 + x))] = 0x00000000;
                            pixels[(((bg_tile_y * 8 + y) * 2) * 160 + (bg_tile_x * 8 + x + 1))] = 0x00000000;
                            pixels[((((bg_tile_y * 8 + y) * 2) + 1) * 160 + (bg_tile_x * 8 + x))] = 0x00000000;
                            pixels[((((bg_tile_y * 8 + y) * 2) + 1) * 160 + (bg_tile_x * 8 + x + 1))] = 0x00000000;
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
