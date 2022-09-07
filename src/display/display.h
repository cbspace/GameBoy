#ifndef DISPLAY_H
#define DISPLAY_H

#include "displayconst.h"
#include "../memory/memory.h"
#include "../render/render.h"
#include "../interrupt/interrupt.h"
#include "../clock/clock.h"

#include <string>
#include <stdint.h>
#include <SDL2/SDL.h>

// Display Class looks after SDL Window
// Render Class looks after pixel array

enum class ColourValues {
    C0,
    C1,
    C2,
    C3
};

class Display
{
    public:
        Display();                              // Constructor
        uint8_t* pixels;						// Array of pixels to display
        void attach_memory(Memory* mem_in);     // Set pointer used to access memory object
        void attach_render(Render* ren_in);     // Set pointer used to access render object
        void attach_interrupt(Interrupt* interrupt_in); // Set pointer used to access interrupt object
        void attach_clock(Clock* clk_in);      // Set pointer used to access clock object

        int8_t init();                          // Set up SDL
        void set_title(string title_add);       // Set title of SDL window
        void display_cycle();                   // Called from the main loop, a single display cycle
        void update_line();                     // Update the current line
        void draw_frame();                      // Draw frame to display
        virtual ~Display();                     // Destructor

    private:
        Memory* mem;                            // Pointer to memory object
        Render* ren;                            // Pointer to renderer object
        Interrupt* ir;                          // Pointer to interrupt object
        Clock* clk;								// Pointer to clock object
        uint32_t* display_buffer;				// Array of pixels for buffer

        void update_stat_reg(uint8_t mode_val); // Update STAT flags
        void colour();							// Add colour data to pixel
        void scale();							// Scale the pixels to match buffer size
        void clear_pixels();					// Clear pixel array

        // SDl Objects
        SDL_Window* window;
        SDL_Surface* drawSurface;
        SDL_Renderer* sdlRenderer;
        SDL_Texture* texture;

};

#endif // DISPLAY_H
