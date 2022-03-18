#ifndef RENDER_H
#define RENDER_H

#include "memory.h"
#include "displayconst.h"

#include <string>
#include <stdint.h>

// Display Class looks after SDL Window
// Render Class looks after pixel array

/* logo_data is char* array read from the ROM (48 bytes long)
    Data Format:
    Gameboy tiles are 8x8 pixels in size. An 8x8 image is
    represented by 16 bytes. There are 4 'colours' that
    can be shown at each pixel: eg:
    ..1331..   ->       00111100
                        00011000
*/

using namespace std;

class Render
{
    public:
	    uint8_t* pixels;					   // Array of pixels to display

        Render();                              // Constructor
        void attach_memory(Memory* mem_in);    // Set pointer used to access memory object
        void attach_pixels(uint8_t* pix_in);   // Set pointer to pixel array
        virtual ~Render();                     // Destructor

        // Rendering
        void render_line(uint8_t y);           // Render a single line

    private:
        Memory* mem;                            // Pointer to memory object

        void draw_bg_line(uint8_t line_y);      // Draw bg tiles into pixel array
        uint8_t get_bg_pixel(uint8_t bg_y, uint8_t bg_x, uint16_t bg_tdt, uint8_t bg_tm);  // Get a bg pixel from a tile
};

#endif // RENDER_H
