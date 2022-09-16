#include "display.h"
#include "displayconst.h"
#include "memory.h"
#include "interrupt.h"
#include "clock.h"

#include <iostream>
#include "lib/Types.h"
#include <string>
#include <SDL2/SDL.h>

using namespace std;

Display::Display(Memory& mem_in, Interrupt& ir_in, Clock& clk_in) :
    mem(mem_in),
    ren(mem_in, pixels),
    ir(ir_in),
    clk(clk_in),
    window(NULL),
    drawSurface(NULL),
    sdlRenderer(NULL),
    texture(NULL)
{}

void Display::set_window_title(string title_add)
{
    if (title_add.empty()) {
        return;
    }

    string window_title_const(WINDOW_TITLE);
    string new_window_title = window_title_const + " - " + title_add;
    SDL_SetWindowTitle(window, new_window_title.c_str());
}

void Display::display_cycle()
{
    u8 current_modeB0 = mem.get_bit(R_LCDSTAT, R_STAT_MODE_B0);
    u8 current_modeB1 = mem.get_bit(R_LCDSTAT, R_STAT_MODE_B1);
    DisplayMode current_mode = static_cast<DisplayMode>((current_modeB1 << 1) + current_modeB0);

    u8 ly_val = mem.get_byte(R_LY);

    // if (current_mode == DisplayMode::Mode0) {
    //     if (clk.cycles_reached(DisplayMode::Mode0)) {
    //         if (ly_val == DISP_H)
    //         {
    //             update_stat_reg(static_cast<u8>(DisplayMode::Mode1));
    //         } else {
    //             update_stat_reg(static_cast<u8>(DisplayMode::Mode2));
    //         }
    //     }
    // } else if (current_mode == DisplayMode::Mode1) {
    //     if (clk.cycles_reached(DisplayMode::Mode1)) {
    //         update_stat_reg(static_cast<u8>(DisplayMode::Mode2));
    //     }
    // } else if (current_mode == DisplayMode::Mode2) {
    //     if (clk.cycles_reached(DisplayMode::Mode2)) {
    //         update_stat_reg(static_cast<u8>(DisplayMode::Mode3));
    //         update_line();
    //     }
    // } else if (current_mode == DisplayMode::Mode3) {
    //     if (clk.cycles_reached(DisplayMode::Mode3)) {
    //         update_stat_reg(static_cast<u8>(DisplayMode::Mode0));
    //         clk.reset_cycles();
    //     }
    // }



    if (clk.cycles_reached(DisplayMode::Mode3))
    {
        if (current_mode != DisplayMode::Mode3)
        {
            if (ly_val < DISP_H)
            {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode3));
            } else {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode1));
            }

            update_line();
            clk.reset_cycles();
        }
    }
    else if (clk.cycles_reached(DisplayMode::Mode2))
    {
        if (current_mode != DisplayMode::Mode2)
        {
            if (ly_val < DISP_H)
            {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode2));
            } else {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode1));
            }
        }
    }
    else if (clk.cycles_reached(DisplayMode::Mode0))
    {
        if (current_mode != DisplayMode::Mode0)
        {
            if (ly_val < DISP_H)
            {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode0));
            } else {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode1));
            }
        }
    }

}

void Display::update_line()
{
	u8 ly_val;

    ly_val = mem.get_byte(R_LY);

    if (ly_val < DISP_H)
    {
        ren.render_line(ly_val);
        mem.write_byte(R_LY, ly_val + 1);
    }
    else if (ly_val == DISP_H)
    {
    	draw_frame();
        ir.if_update(I_VBLANK, true);
        mem.write_byte(R_LY, ly_val + 1);
    }
    // During V-Blank
    else if (ly_val < LCD_Y_MAX)
    {
        mem.write_byte(R_LY, ly_val + 1);
    }
    // After V-Blank
    else
    {
        mem.write_byte(R_LY, 0x00);
        clear_pixels();
        ren.refresh_sprites();
    }
}

void Display::update_stat_reg(u8 mode_val)
{
	u8 lyc_val, lyc_sel_val, mode_10_sel_val, mode_01_sel_val, mode_00_sel_val;
    u8 ly_val = mem.get_byte(R_LY);

    lyc_val = mem.get_byte(R_LYC);

    lyc_sel_val = mem.get_bit(R_LCDSTAT, R_STAT_LYC_IR);
    mode_10_sel_val = mem.get_bit(R_LCDSTAT, R_STAT_MODE_10_IR);
    mode_01_sel_val = mem.get_bit(R_LCDSTAT, R_STAT_MODE_01_IR);
    mode_00_sel_val = mem.get_bit(R_LCDSTAT, R_STAT_MODE_00_IR);

    if (ly_val == lyc_val)
    {
    	mem.write_bit(R_LCDSTAT, R_STAT_LYC_FLAG, 1);
    	if (lyc_sel_val)
    	{
            ir.if_update(I_LCDSTAT, true);
    	}
    }
    else
    {
    	mem.write_bit(R_LCDSTAT, R_STAT_LYC_FLAG, 0);
    }

    if (mode_val == 0x00)
    {
    	mem.write_bit(R_LCDSTAT, R_STAT_MODE_B1, 0);
    	mem.write_bit(R_LCDSTAT, R_STAT_MODE_B0, 0);
    	if (mode_00_sel_val)
    	{
            ir.if_update(I_LCDSTAT, true);
    	}
    }
    else if (mode_val == 0x01)
    {
    	mem.write_bit(R_LCDSTAT, R_STAT_MODE_B1, 0);
    	mem.write_bit(R_LCDSTAT, R_STAT_MODE_B0, 1);
    	if (mode_01_sel_val)
    	{
            ir.if_update(I_LCDSTAT, true);
    	}
    }
    else if (mode_val == 0x02)
    {
    	mem.write_bit(R_LCDSTAT, R_STAT_MODE_B1, 1);
    	mem.write_bit(R_LCDSTAT, R_STAT_MODE_B0, 0);
    	if (mode_10_sel_val)
    	{
            ir.if_update(I_LCDSTAT, true);
    	}
    }
    else if (mode_val == 0x03)
    {
    	mem.write_bit(R_LCDSTAT, R_STAT_MODE_B1, 1);
    	mem.write_bit(R_LCDSTAT, R_STAT_MODE_B0, 1);
    }

}

void Display::draw_frame()
{
    colour();
    scale();

    SDL_UpdateTexture(texture, NULL, display_buffer, width * sizeof(u32));
    SDL_RenderClear(sdlRenderer);
    SDL_RenderCopy(sdlRenderer, texture, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);

    clk.frame_delay();
}

void Display::colour()
{
	u32 current_pixel_ref;

    for (u16 y = 0; y < DISP_H; y++)
    {
        for (u16 x = 0; x < DISP_W; x++)
        {
        	current_pixel_ref = y*DISP_W + x;
        	switch(pixels[current_pixel_ref])
        	{
        	case ColourValue::C0:
        		pixels_coloured[current_pixel_ref] = COLOUR_BG_ARGB;
        		break;
        	case ColourValue::C1:
        		pixels_coloured[current_pixel_ref] = COLOUR_C1_ARGB;
        		break;
        	case ColourValue::C2:
        		pixels_coloured[current_pixel_ref] = COLOUR_C2_ARGB;
        		break;
        	case ColourValue::C3:
        		pixels_coloured[current_pixel_ref] = COLOUR_C3_ARGB;
        		break;
        	default:
        		pixels_coloured[current_pixel_ref] = 0xffffffff;
        		break;
        	}

        }
    }
}

void Display::scale()
{
    for (u16 y = 0; y < DISP_H; y++)
    {
        for (u16 x = 0; x < DISP_W; x++)
        {
            for (u16 y1 = 0; y1 < SCALING_FACTOR; y1++)
            {
                for (u16 x1 = 0; x1 < SCALING_FACTOR; x1++)
                {
                	display_buffer[(y * SCALING_FACTOR + y1) * width + x * SCALING_FACTOR + x1] = pixels_coloured[y * DISP_W + x];
                }
            }
        }
    }
}

void Display::clear_pixels()
{
    for (u16 y = 0; y < DISP_H; y++)
    {
        for (u16 x = 0; x < DISP_W; x++)
        {
            pixels[y * DISP_W + x] = ColourValue::C0;
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

    clear_pixels();

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

	SDL_Quit();
}
