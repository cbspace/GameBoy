#include "render.h"

using namespace std;

Render::Render(Memory& mem_in, ColourValue (&pix_in)[DISP_W*DISP_H]) :
    mem(mem_in),
    pixels(pix_in),
    win_internal_line(0)
{}

void Render::reset_frame()
{
    win_internal_line = 0; // Reset for the start of line 0
}

void Render::render_line(u8 line_y)
{
    // 1. Draw Background
    draw_bg_line(line_y);

    // 2. Draw Window (overlaps BG)
    draw_win_line(line_y);

    // 3. Draw Sprites (respects transparency and priority)
    draw_sprites_line(line_y);
}

void Render::refresh_sprites()
{
    u16 oam_data_addr;

    for (u8 i = 0; i < SPRITE_TILES_MAX; i++)
    {
        oam_data_addr = A_OAM + i * 4;
        // Set y
        spr_att[i].set_sprite_y(mem.get_byte(oam_data_addr));
        // Set x
        spr_att[i].set_sprite_x(mem.get_byte(oam_data_addr + 1));
        // Set tile number
        spr_att[i].set_sprite_tile_no(mem.get_byte(oam_data_addr + 2));
        // Set flags
        spr_att[i].set_sprite_flags(mem.get_byte(oam_data_addr + 3));
    }
}

void Render::draw_sprites_line(u8 line_y)
{
    if (!mem.get_bit(R_LCDC, 1)) return; // Sprites disabled

    bool sprite_16_mode = mem.get_bit(R_LCDC, 2);
    u8 sprite_height = sprite_16_mode ? 16 : 8;

    // Collect visible sprites (DMG limits to 10 per scanline)
    int sprites_on_line = 0;
    u8 line_sprites[10];

    for (u8 i = 0; i < 40 && sprites_on_line < 10; i++)
    {
        u8 oam_y = mem.get_byte(0xFE00 + i * 4);
        if (oam_y == 0 || oam_y >= 160) continue;

        // OAM Y is offset by 16
        int sprite_top = static_cast<int>(oam_y) - 16;
        if (line_y >= sprite_top && line_y < (sprite_top + sprite_height))
        {
            line_sprites[sprites_on_line++] = i;
        }
    }

    // Render sprites in reverse order so lower OAM indices take priority on DMG
    for (int idx = sprites_on_line - 1; idx >= 0; idx--)
    {
        u16 oam_addr = 0xFE00 + line_sprites[idx] * 4;
        u8 oam_y     = mem.get_byte(oam_addr);
        u8 oam_x     = mem.get_byte(oam_addr + 1);
        u8 tile_no   = mem.get_byte(oam_addr + 2);
        u8 flags     = mem.get_byte(oam_addr + 3);

        if (oam_x == 0 || oam_x >= 168) continue;

        int screen_x = static_cast<int>(oam_x) - 8;
        int screen_y = static_cast<int>(oam_y) - 16;

        bool flip_y = (flags & 0x40);
        bool flip_x = (flags & 0x20);
        bool priority_bg = (flags & 0x80);
        u8 palette = mem.get_byte((flags & 0x10) ? 0xFF49 : 0xFF48);

        u8 row = line_y - screen_y;
        if (flip_y) row = (sprite_height - 1) - row;

        if (sprite_16_mode)
        {
            tile_no &= 0xFE;
            if (row >= 8)
            {
                tile_no += 1;
                row -= 8;
            }
        }

        u16 tile_addr = 0x8000 + (tile_no * 16) + (row * 2);
        u8 low_byte  = mem.get_byte(tile_addr);
        u8 high_byte = mem.get_byte(tile_addr + 1);

        for (u8 col = 0; col < 8; col++)
        {
            int pixel_x = screen_x + col;
            if (pixel_x < 0 || pixel_x >= DISP_W) continue;

            u8 bit = flip_x ? col : (7 - col);
            u8 color_bit_0 = (low_byte >> bit) & 0x01;
            u8 color_bit_1 = (high_byte >> bit) & 0x01;
            u8 color_index = (color_bit_1 << 1) | color_bit_0;

            // Color 0 is transparent for sprites
            if (color_index == 0) continue;

            // Priority check against BG: if priority bit is 1, sprite only displays over BG color index 0
            if (priority_bg && raw_bg_color_index[pixel_x] != 0) continue;

            u8 final_shade = (palette >> (color_index * 2)) & 0x03;
            pixels[line_y * DISP_W + pixel_x] = static_cast<ColourValue>(final_shade);
        }
    }
}

ColourValue Render::get_sprite_pixel(u8 tile_no, u8 sprite_y, u8 sprite_x)
{
    // Calculate start address for tile
    u16 tile_addr = A_TDT1 + tile_no * 16;

    u8 byte_number = sprite_y * 2;
    u8 bit_number = 7 - sprite_x;

    // Load data for tile byte
    u8 tile_byte1 = mem.get_byte(tile_addr + byte_number);
    u8 tile_byte2 = mem.get_byte(tile_addr + byte_number + 1);

    return static_cast<ColourValue>(((tile_byte2 & (1<<(bit_number))) >> (bit_number) << 1)
           + ((tile_byte1 & (1<<(bit_number))) >> (bit_number)));
}

void Render::draw_bg_line(u8 line_y)
{
    u16 map_base = mem.get_bit(R_LCDC, 3) ? 0x9C00 : 0x9800;
    bool unsigned_tile_addressing = (mem.get_bit(R_LCDC, 4) == 1);

    u8 scroll_x = mem.get_byte(R_SCX);
    u8 scroll_y = mem.get_byte(R_SCY);
    u8 bg_palette = mem.get_byte(0xFF47);

    u8 pix_y = scroll_y + line_y;
    u8 tile_row = (pix_y / 8) % 32;
    u8 tile_y_offset = (pix_y % 8) * 2;

    for (u8 x = 0; x < DISP_W; x++)
    {
        u8 pix_x = scroll_x + x;
        u8 tile_col = (pix_x / 8) % 32;
        u8 bit_number = 7 - (pix_x % 8);

        u16 map_address = map_base + (tile_row * 32) + tile_col;
        u8 raw_tile_id = mem.get_byte(map_address);

        u16 tile_data_addr;
        if (unsigned_tile_addressing)
        {
            tile_data_addr = 0x8000 + (raw_tile_id * 16);
        }
        else
        {
            int8_t signed_id = static_cast<int8_t>(raw_tile_id);
            tile_data_addr = static_cast<u16>(0x9000 + (signed_id * 16));
        }

        u8 low_byte  = mem.get_byte(tile_data_addr + tile_y_offset);
        u8 high_byte = mem.get_byte(tile_data_addr + tile_y_offset + 1);

        u8 color_bit_0 = (low_byte >> bit_number) & 0x01;
        u8 color_bit_1 = (high_byte >> bit_number) & 0x01;
        u8 color_index = (color_bit_1 << 1) | color_bit_0;

        // Record the raw index for sprite priority checks
        raw_bg_color_index[x] = color_index;

        u8 final_shade = (bg_palette >> (color_index * 2)) & 0x03;
        pixels[line_y * DISP_W + x] = static_cast<ColourValue>(final_shade);
    }
}

void Render::draw_win_line(u8 line_y)
{
    // Window is enabled via LCDC Bit 5
    if (!mem.get_bit(R_LCDC, 5))
    {
        return;
    }

    u8 wy = mem.get_byte(0xFF4A);
    u8 wx = mem.get_byte(0xFF4B);

    // The window only triggers if line_y >= WY and WX is within visible range (0 to 166)
    if (line_y < wy || wx > 166)
    {
        return;
    }

    u16 map_base = mem.get_bit(R_LCDC, 6) ? 0x9C00 : 0x9800;
    bool unsigned_tile_addressing = (mem.get_bit(R_LCDC, 4) == 1);
    u8 bg_palette = mem.get_byte(0xFF47);

    // Use win_internal_line (not line_y) to calculate the vertical tile row
    u8 tile_row = (win_internal_line / 8) % 32;
    u8 tile_y_offset = (win_internal_line % 8) * 2;

    u8 win_pixel_x = 0;
    for (u8 x = 0; x < DISP_W; x++)
    {
        // Window triggers horizontally when screen X + 7 >= WX
        if (x + 7 >= wx)
        {
            u8 tile_col = (win_pixel_x / 8) % 32;
            u8 bit_number = 7 - (win_pixel_x % 8);

            u16 map_address = map_base + (tile_row * 32) + tile_col;
            u8 raw_tile_id = mem.get_byte(map_address);

            u16 tile_data_addr;
            if (unsigned_tile_addressing)
            {
                tile_data_addr = 0x8000 + (raw_tile_id * 16);
            }
            else
            {
                int8_t signed_id = static_cast<int8_t>(raw_tile_id);
                tile_data_addr = static_cast<u16>(0x9000 + (signed_id * 16));
            }

            u8 low_byte  = mem.get_byte(tile_data_addr + tile_y_offset);
            u8 high_byte = mem.get_byte(tile_data_addr + tile_y_offset + 1);

            u8 color_bit_0 = (low_byte >> bit_number) & 0x01;
            u8 color_bit_1 = (high_byte >> bit_number) & 0x01;
            u8 color_index = (color_bit_1 << 1) | color_bit_0;

            raw_bg_color_index[x] = color_index;

            u8 final_shade = (bg_palette >> (color_index * 2)) & 0x03;
            pixels[line_y * DISP_W + x] = static_cast<ColourValue>(final_shade);

            win_pixel_x++;
        }
    }

    // Only increment when the window actually rendered pixels on this line
    win_internal_line++;
}

Render::~Render() {}
