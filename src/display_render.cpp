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

    /// Temp fill canvas
    for (uint16_t j = 0; j < height; j++)
    {
        for (uint16_t i = 0; i < width; i++)
        {
            pixels[j*width + i] = BG_COLOUR_ARGB;
        }
    }

    /// Draw logo
    uint8_t lg[24];
    /*lg[0] = 0x3c;
    lg[1] = 0x42;
    lg[2] = 0xb9;
    lg[3] = 0xa5;
    lg[4] = 0xb9;
    lg[5] = 0xa5;
    lg[6] = 0x42;
    lg[7] = 0x3c;*/



    for (uint16_t b = 0; b < 12; b++)
    {

        for (int z = 0; z < 8; z++)
        {
            lg[z] = mem->get_byte(0x8010 + z * 2 + b*16);
        }

        for (uint16_t y = 0; y < 8; y++)
        {
            for (uint16_t x = 0; x < 8; x++)
            {
                if ((lg[y] & (1<<(7-x))) >> (7-x) == 1)
                {
                    pixels[y*2*width + 2*(x+b*8)] = 0x00000000;
                    pixels[y*2*width + 2*(x+b*8) + 1] = 0x00000000;
                    pixels[(y*2+1)*width + 2*(x+b*8)] = 0x00000000;
                    pixels[(y*2+1)*width + 2*(x+b*8) + 1] = 0x00000000;
                }
            }
        }
    }

    for (uint16_t b = 0; b < 12; b++)
    {

        for (int z = 0; z < 8; z++)
        {
            lg[z] = mem->get_byte(0x8090 + z * 2 + b*16);
        }

        for (uint16_t y = 0; y < 8; y++)
        {
            for (uint16_t x = 0; x < 8; x++)
            {
                if ((lg[y] & (1<<(7-x))) >> (7-x) == 1)
                {
                    pixels[y*2*width + 2*(x+b*8)] = 0x00000000;
                    pixels[y*2*width + 2*(x+b*8) + 1] = 0x00000000;
                    pixels[(y*2+1)*width + 2*(x+b*8)] = 0x00000000;
                    pixels[(y*2+1)*width + 2*(x+b*8) + 1] = 0x00000000;
                }
            }
        }
    }
    /*

    // Draw background tiles
    for (uint8_t bg_tile_x = 0; bg_tile_x < BG_TILES_MAX; bg_tile_x++)
    {
            for (uint8_t bg_tile_y = 0; bg_tile_y < BG_TILES_MAX; bg_tile_y++)
            {

            }
    }*/

    // Update texture
    SDL_UpdateTexture(texture, NULL, pixels, width * sizeof(uint32_t));

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
