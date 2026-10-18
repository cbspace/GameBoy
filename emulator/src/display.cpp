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

void GBDisplay::display_cycle(u32 cycles)
{
    u8 lcdc_val = mem.get_byte(R_LCDC);
    if ((lcdc_val & 0x80) == 0)
    {
        line_clock_cycles = 0;
        return;
    }

    new_frame_drawn = false;
    line_clock_cycles += static_cast<i32>(cycles);

    u8 current_modeB0 = mem.get_bit(R_LCDSTAT, u8(StatReg::MODE_B0));
    u8 current_modeB1 = mem.get_bit(R_LCDSTAT, u8(StatReg::MODE_B1));
    DisplayMode current_mode = static_cast<DisplayMode>((current_modeB1 << 1) + current_modeB0);
    u8 ly_val = mem.get_byte(R_LY);

    switch (current_mode)
    {
    case DisplayMode::Mode2:
    {
        // On DMG, the first scanline after LCD enable has a slightly longer Mode 2 window (92 dots vs 80)
        i32 mode2_limit = first_scanline_after_enable ? 92 : 80;
        if (line_clock_cycles >= mode2_limit)
        {
            update_stat_reg(static_cast<u8>(DisplayMode::Mode3));
        }
        break;
    }

    case DisplayMode::Mode3:
        if (line_clock_cycles >= 252)
        {
            if (ly_val < DISP_H)
            {
                ren.render_line(ly_val);
            }
            update_stat_reg(static_cast<u8>(DisplayMode::Mode0));
        }
        break;

    case DisplayMode::Mode0:
    {
        // Scanline 0 after enable finishes early (440 dots) before advancing to LY = 1
        i32 scanline_limit = first_scanline_after_enable ? 440 : 456;
        if (line_clock_cycles >= scanline_limit)
        {
            line_clock_cycles -= scanline_limit;
            first_scanline_after_enable = false;

            ly_val++;
            mem.write_byte(R_LY, ly_val);

            if (ly_val == DISP_H)
            {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode1));
                ir.request_interrupt(InterruptType::VBlank);
                draw_frame();
            }
            else
            {
                update_stat_reg(static_cast<u8>(DisplayMode::Mode2));
            }
        }
        break;
    }

    case DisplayMode::Mode1: // V-Blank: Lines 144 -> 153 (456 dots each)
        if (line_clock_cycles >= 456)
        {
            line_clock_cycles -= 456;
            ly_val++;

            if (ly_val > 153)
            {
                ly_val = 0;
                mem.write_byte(R_LY, 0);
                update_stat_reg(static_cast<u8>(DisplayMode::Mode2));
                ren.reset_frame();
                ren.refresh_sprites();
            }
            else
            {
                mem.write_byte(R_LY, ly_val);
                update_stat_reg(static_cast<u8>(DisplayMode::Mode1));
            }
        }
        break;
    }
}

void GBDisplay::notify_lcdc_write(u8 new_lcdc)
{
    bool was_on = lcd_enabled;
    bool is_on = (new_lcdc & 0x80) != 0;
    lcd_enabled = is_on;

    if (!was_on && is_on)
    {
        // Cancel the 12-cycle write instruction delay from main_loop
        line_clock_cycles = -12;
        mem.write_byte(R_LY, 0);

        first_scanline_after_enable = true;

        // Force STAT to Mode 2 immediately
        update_stat_reg(static_cast<u8>(DisplayMode::Mode2));
    }
    else if (was_on && !is_on)
    {
        line_clock_cycles = 0;
        mem.write_byte(R_LY, 0);
        first_scanline_after_enable = false;

        // Mode 0 when LCD off on DMG
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B1), false);
        mem.write_bit(R_LCDSTAT, u8(StatReg::MODE_B0), false);
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
