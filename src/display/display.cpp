#include "display.h"
#include "displayconst.h"
#include "../memory/memory.h"
#include "../interrupt/interrupt.h"
#include "../clock/clock.h"

#include <iostream>
#include <stdint.h>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

Display::Display()
{
    window = NULL;
    drawSurface = NULL;
    sdlRenderer = NULL;
    display_buffer = NULL;
    pixels = NULL;
    ren = NULL;
    mem = NULL;
    texture = NULL;
    ir = NULL;
    clk = NULL;
}

void Display::attach_memory(Memory* mem_in)
{
    mem = mem_in;
}

void Display::attach_render(Render* ren_in)
{
    ren = ren_in;
}

void Display::attach_interrupt(Interrupt* interrupt_in)
{
    ir = interrupt_in;
}

void Display::attach_clock(Clock* clock_in)
{
    clk = clock_in;
}

void Display::set_title(string title_add)
{
    string window_title_const(WINDOW_TITLE);
    if (title_add.length() > 0) {
    	string new_window_title = window_title_const + " - " + title_add;
    	SDL_SetWindowTitle(window, new_window_title.c_str());
    }
    else {
    	SDL_SetWindowTitle(window, window_title_const.c_str());
    }
}

// Called from the main loop, a single display cycle
// Used to update display modes
void Display::display_cycle()
{
    uint8_t current_modeB0 = mem->get_bit(R_LCDSTAT, R_STAT_MODE_B0);
    uint8_t current_modeB1 = mem->get_bit(R_LCDSTAT, R_STAT_MODE_B1);
    uint8_t current_mode = (current_modeB1 << 1) + current_modeB0;

	// Get current LY value from register
    uint8_t ly_val = mem->get_byte(R_LY);

    //cout << unsigned(current_mode) << endl;
    //cout << unsigned(ly_val) << endl;

    // ISSUE - 3's appear on LY = 144 then changes to 1
    if (clk->cycles_reached(CLK_DISPLAY_MODE3))
    {
        if (current_mode != CLK_DISPLAY_MODE3)
        {
            if (ly_val < DISP_H)
            {
                update_stat_reg(CLK_DISPLAY_MODE3);
            } else {
                update_stat_reg(CLK_DISPLAY_MODE1);
            }

            // Draw current line
            update_line();

            // Debug
            //mem->ram_debug(A_TDT1, A_OAM);

            clk->reset_cycles();
        }
    }
    else if (clk->cycles_reached(CLK_DISPLAY_MODE2))
    {
        if (current_mode != CLK_DISPLAY_MODE2)
        {
            if (ly_val < DISP_H)
            {
                update_stat_reg(CLK_DISPLAY_MODE2);
            } else {
                update_stat_reg(CLK_DISPLAY_MODE1);
            }
        }
    }
    else if (clk->cycles_reached(CLK_DISPLAY_MODE0))
    {
        if (current_mode != CLK_DISPLAY_MODE0)
        {
            if (ly_val < DISP_H)
            {
                update_stat_reg(CLK_DISPLAY_MODE0);
            } else {
                update_stat_reg(CLK_DISPLAY_MODE1);
            }
        }
    }

}

// Update the current line
void Display::update_line()
{
	uint8_t ly_val;

	// Get current LY value from register
    ly_val = mem->get_byte(R_LY);

    // Draw line on display
    if (ly_val < DISP_H)
    {
        // Render line
        ren->render_line(ly_val);

        // Update LY register
        mem->write_byte(R_LY, ly_val + 1);
    }
    // Start of V-Blank, draw frame
    else if (ly_val == DISP_H)
    {
        // Draw the frame
    	draw_frame();

    	// Set VBLANK interrupt flag
        ir->if_update(I_VBLANK, true);

        // Update LY register
        mem->write_byte(R_LY, ly_val + 1);
    }
    // During V-Blank
    else if (ly_val < LCD_Y_MAX)
    {
        // Update LY register
        mem->write_byte(R_LY, ly_val + 1);
    }
    // After V-Blank
    else
    {
        // Reset LY register
        mem->write_byte(R_LY, 0x00);

        // Clear pixels
        clear_pixels();

        // Refresh sprites
        ren->refresh_sprites();
    }
}

// Update STAT flags
void Display::update_stat_reg(uint8_t mode_val)
{
	uint8_t lyc_val, lyc_sel_val, mode_10_sel_val, mode_01_sel_val, mode_00_sel_val;

	// Get current LY value from register
    uint8_t ly_val = mem->get_byte(R_LY);

    // Get LYC value
    lyc_val = mem->get_byte(R_LYC);

    // Get Interrupt select values
    lyc_sel_val = mem->get_bit(R_LCDSTAT, R_STAT_LYC_IR);
    mode_10_sel_val = mem->get_bit(R_LCDSTAT, R_STAT_MODE_10_IR);
    mode_01_sel_val = mem->get_bit(R_LCDSTAT, R_STAT_MODE_01_IR);
    mode_00_sel_val = mem->get_bit(R_LCDSTAT, R_STAT_MODE_00_IR);

    // Update Coincidence flag in STAT register
    if (ly_val == lyc_val)
    {
    	mem->write_bit(R_LCDSTAT, R_STAT_LYC_FLAG, 1);
    	if (lyc_sel_val)
    	{
        	// Set LCDSTAT interrupt flag
            ir->if_update(I_LCDSTAT, true);
    	}
    }
    else
    {
    	mem->write_bit(R_LCDSTAT, R_STAT_LYC_FLAG, 0);
    }

    // Update Mode flags in STAT register and update IR flags
    if (mode_val == 0x00)
    {
    	mem->write_bit(R_LCDSTAT, R_STAT_MODE_B1, 0);
    	mem->write_bit(R_LCDSTAT, R_STAT_MODE_B0, 0);
    	if (mode_00_sel_val)
    	{
        	// Set LCDSTAT interrupt flag
            ir->if_update(I_LCDSTAT, true);
    	}
    }
    else if (mode_val == 0x01)
    {
    	mem->write_bit(R_LCDSTAT, R_STAT_MODE_B1, 0);
    	mem->write_bit(R_LCDSTAT, R_STAT_MODE_B0, 1);
    	if (mode_01_sel_val)
    	{
        	// Set LCDSTAT interrupt flag
            ir->if_update(I_LCDSTAT, true);
    	}
    }
    else if (mode_val == 0x02)
    {
    	mem->write_bit(R_LCDSTAT, R_STAT_MODE_B1, 1);
    	mem->write_bit(R_LCDSTAT, R_STAT_MODE_B0, 0);
    	if (mode_10_sel_val)
    	{
        	// Set LCDSTAT interrupt flag
            ir->if_update(I_LCDSTAT, true);
    	}
    }
    else if (mode_val == 0x03)
    {
    	mem->write_bit(R_LCDSTAT, R_STAT_MODE_B1, 1);
    	mem->write_bit(R_LCDSTAT, R_STAT_MODE_B0, 1);
    }

}

void Display::draw_frame()
{
    scale();
    colour();

    SDL_UpdateTexture(texture, NULL, display_buffer, width * sizeof(uint32_t));
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);

    clk->frame_delay();
}

void Display::colour()
{
	uint32_t current_pixel_ref;

    for (uint16_t j = 0; j < height; j++)
    {
        for (uint16_t i = 0; i < width; i++)
        {
        	current_pixel_ref = j*width + i;
        	switch(display_buffer[current_pixel_ref])
        	{
        	case 0x00:
        		display_buffer[current_pixel_ref] = COLOUR_BG_ARGB;
        		break;
        	case 0x01:
        		display_buffer[current_pixel_ref] = COLOUR_C1_ARGB;
        		break;
        	case 0x02:
        		display_buffer[current_pixel_ref] = COLOUR_C2_ARGB;
        		break;
        	case 0x03:
        		display_buffer[current_pixel_ref] = COLOUR_C3_ARGB;
        		break;
        	default:
        		display_buffer[current_pixel_ref] = 0xffffffff;
        		break;
        	}

        }
    }
}

void Display::scale()
{
    for (uint16_t y = 0; y < DISP_H; y++)
    {
        for (uint16_t x = 0; x < DISP_W; x++)
        {
            for (uint16_t y1 = 0; y1 < SCALING_FACTOR; y1++)
            {
                for (uint16_t x1 = 0; x1 < SCALING_FACTOR; x1++)
                {
                	display_buffer[(y * SCALING_FACTOR + y1) * width + x * SCALING_FACTOR + x1] = pixels[y * DISP_W + x];
                }
            }
        }
    }
}

void Display::clear_pixels()
{
    for (uint16_t y = 0; y < DISP_H; y++)
    {
        for (uint16_t x = 0; x < DISP_W; x++)
        {
            pixels[y * DISP_W + x] = 0;
        }
    }
}

optional<Error> Display::init()
{
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) { return Error("SDL unable to initialise! SDL_Error", SDL_GetError()); }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL);
    if (window == NULL) { return Error("Unable to create SDL Window! SDL_Error", SDL_GetError()); }

    sdlRenderer = SDL_CreateRenderer(window, -1, 0);
    if ( sdlRenderer == NULL ) { return Error("Unable to create SDL Renderer! SDL_Error", SDL_GetError()); };

    texture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);

    display_buffer = new uint32_t[width*height];
    pixels = new uint8_t[width*height];
    clear_pixels();

    ren->attach_pixels(pixels);

    return nullopt;
}

Display::~Display()
{
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(sdlRenderer);
    SDL_DestroyWindow(window);

	texture = NULL;
	sdlRenderer = NULL;
	window = NULL;

	delete[] display_buffer;

	SDL_Quit();
}
