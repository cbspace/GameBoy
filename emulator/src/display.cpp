#include "display.h"

using namespace std;

GBDisplay::GBDisplay(Memory& mem_in, Interrupt& ir_in) :
    mem(mem_in),
    ren(mem_in, pixels),
    ir(ir_in),
    new_frame_drawn(false),
    line_clock_cycles(0),
    pixels_coloured(new u32[DISP_W*DISP_H])
{
    clear_pixels();
}

bool GBDisplay::new_frame_is_drawn() {
    return new_frame_drawn;
}

// void GBDisplay::set_window_title(string title_add)
// {
//     if (title_add.empty()) {
//         return;
//     }

//     string window_title_const(WINDOW_TITLE);
//     string new_window_title = window_title_const + " - " + title_add;
//     SDL_SetWindowTitle(window, new_window_title.c_str());
// }

bool GBDisplay::cycles_reached(DisplayMode display_mode) const
{
    switch (display_mode)
    {
    case DisplayMode::Mode2:
        // Mode 2 ends at 80 dots
        return line_clock_cycles >= 80;

    case DisplayMode::Mode3:
        // Mode 3 ends at 80 + 172 = 252 dots
        return line_clock_cycles >= (80 + 172);

    case DisplayMode::Mode0:
        // Mode 0 ends at 456 dots (full scanline complete)
        return line_clock_cycles >= 456;

    case DisplayMode::Mode1:
        // Each V-Blank line (lines 144-153) lasts a full scanline (456 dots)
        return line_clock_cycles >= 456;

    default:
        return false;
    }
}

void GBDisplay::display_cycle(u32 cycles)
{
    new_frame_drawn = false;
    line_clock_cycles += cycles;

    u8 current_modeB0 = mem.get_bit(R_LCDSTAT, u8(StatReg::MODE_B0));
    u8 current_modeB1 = mem.get_bit(R_LCDSTAT, u8(StatReg::MODE_B1));
    DisplayMode current_mode = static_cast<DisplayMode>((current_modeB1 << 1) + current_modeB0);
    u8 ly_val = mem.get_byte(R_LY);

    switch (current_mode)
    {
    case DisplayMode::Mode2: // OAM Search (80 cycles)
        if (cycles_reached(DisplayMode::Mode2))
        {
            update_stat_reg(static_cast<u8>(DisplayMode::Mode3));
        }
        break;

    case DisplayMode::Mode3: // Pixel Transfer (172 cycles)
        if (cycles_reached(DisplayMode::Mode3))
        {
            // Render the line at the end of Mode 3
            if (ly_val < DISP_H)
            {
                ren.render_line(ly_val);
            }
            update_stat_reg(static_cast<u8>(DisplayMode::Mode0));
        }
        break;

    case DisplayMode::Mode0: // H-Blank (204 cycles)
        if (cycles_reached(DisplayMode::Mode0))
        {
            ly_val++;
            mem.write_byte(R_LY, ly_val);

            if (ly_val == DISP_H) // Reached line 144 -> Enter V-Blank (Mode 1)
            {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode1));
                ir.request_interrupt(InterruptType::VBlank);
                draw_frame(); // Convert pixels to RGB and set new_frame_drawn = true
            }
            else // Next visible scanline -> Mode 2
            {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode2));
            }
            line_clock_cycles = 0;
        }
        break;

    case DisplayMode::Mode1: // V-Blank (456 cycles per line across lines 144-153)
        if (cycles_reached(DisplayMode::Mode1))
        {
            ly_val++;
            if (ly_val > 153)
            {
                // Frame complete, wrap back to scanline 0 and Mode 2
                ly_val = 0;
                mem.write_byte(R_LY, ly_val);
                update_stat_reg(static_cast<u8>(DisplayMode::Mode2));
                ren.reset_frame();
                ren.refresh_sprites();
            }
            else
            {
                mem.write_byte(R_LY, ly_val);
            }
            line_clock_cycles = 0;
        }
        break;
    }
}

void GBDisplay::update_line()
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
    else if (ly_val < 153)
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

void GBDisplay::update_stat_reg(u8 mode_val)
{
    u8 lyc_val, lyc_sel_val, mode_10_sel_val, mode_01_sel_val, mode_00_sel_val;
    u8 ly_val = mem.get_byte(R_LY);

    lyc_val = mem.get_byte(R_LYC);

    lyc_sel_val = mem.get_bit(R_LCDSTAT, u8(StatReg::LYC_IR));
    mode_10_sel_val = mem.get_bit(R_LCDSTAT, u8(StatReg::MODE_10_IR));
    mode_01_sel_val = mem.get_bit(R_LCDSTAT, u8(StatReg::MODE_01_IR));
    mode_00_sel_val = mem.get_bit(R_LCDSTAT, u8(StatReg::MODE_00_IR));

    if (ly_val == lyc_val)
    {
        mem.write_bit(R_LCDSTAT, u8(StatReg::LYC_FLAG), true);
        if (lyc_sel_val)
        {
            ir.if_update(I_LCDSTAT, true);
        }
    }
    else
    {
        mem.write_bit(R_LCDSTAT, u8(StatReg::LYC_FLAG), false);
    }

    if (mode_val == 0x00)
    {
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B1), false);
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B0), false);
        if (mode_00_sel_val)
        {
            ir.if_update(I_LCDSTAT, true);
        }
    }
    else if (mode_val == 0x01)
    {
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B1), false);
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B0), true);
        if (mode_01_sel_val)
        {
            ir.if_update(I_LCDSTAT, true);
        }
    }
    else if (mode_val == 0x02)
    {
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B1), true);
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B0), false);
        if (mode_10_sel_val)
        {
            ir.if_update(I_LCDSTAT, true);
        }
    }
    else if (mode_val == 0x03)
    {
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B1), true);
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B0), true);
    }

}

void GBDisplay::draw_frame()
{
    colour();
    new_frame_drawn = true;
    // Do NOT call clear_pixels() here; preserve the buffer for the Qt renderer!
}

void GBDisplay::colour()
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
                pixels_coloured[current_pixel_ref] = COLOUR_BG_RGB;
                break;
            case ColourValue::C1:
                pixels_coloured[current_pixel_ref] = COLOUR_C1_RGB;
                break;
            case ColourValue::C2:
                pixels_coloured[current_pixel_ref] = COLOUR_C2_RGB;
                break;
            case ColourValue::C3:
                pixels_coloured[current_pixel_ref] = COLOUR_C3_RGB;
                break;
            default:
                pixels_coloured[current_pixel_ref] = 0xffffff;
                break;
            }

        }
    }
}

void GBDisplay::clear_pixels()
{
    for (u16 y = 0; y < DISP_H; y++)
    {
        for (u16 x = 0; x < DISP_W; x++)
        {
            pixels[y * DISP_W + x] = ColourValue::C0;
            pixels_coloured[y * DISP_W + x] = 0x00;
        }
    }
}

u32* GBDisplay::get_buffer() {
    return pixels_coloured;
}

GBDisplay::~GBDisplay()
{}
