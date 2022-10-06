#pragma once

#include "lib/Types.h"

const u32 DISP_W =              160;        // Gameboy display width
const u32 DISP_H =              144;        // Gameboy display height

// Memory Locations - Video
const u32 A_TDT1 =              0x8000;     // Tile Data Table 1 start address (Sprite, BG, Window: 0->255)
const u32 A_TDT2 =              0x8800;     // Tile Data Table 2 start address (BG, Window: -128->127)
const u32 A_BGTM1 =             0x9800;     // Background Tile Map 1 start address
const u32 A_BGTM2 =             0x9C00;     // Background Tile Map 2 start address

// Special Registers - Video
const u32 R_LCDC =              0xff40;     // LCD Control
const u32 R_LCDSTAT =           0xff41;     // LCD Status
const u32 R_SCY =               0xff42;     // Scroll Y
const u32 R_SCX =               0xff43;     // Scroll X
const u32 R_LY =                0xff44;     // LCD Y coordinate
const u32 R_LYC =               0xff45;     // LY Compare
const u32 R_DMA =               0xff46;     // Direct Memory Access
const u32 R_BGP =               0xff47;     // Background Palette
const u32 R_OBP0 =              0xff48;     // Object Palette 0
const u32 R_OBP1 =              0xff49;     // Object Palette 1
const u32 R_WY =                0xff4a;     // Window Y Position
const u32 R_WX =                0xff4b;     // Window X Position

enum class LcdcReg {
    BG_WIN_DISPLAY,
    SPRITE_DISPLAY,
    SPRITE_SIZE,
    BG_TILE_MAP_SEL,
    BG_WIN_TILE_DATA_SEL,
    WIN_DISPLAY,
    WIN_MAP_SEL,
    LCD_CONTROL
};

enum class StatReg {
    MODE_B0,
    MODE_B1,
    LYC_FLAG,
    MODE_00_IR,
    MODE_01_IR,
    MODE_10_IR,
    LYC_IR
};

enum class ColourValue {
    C0, // Lightest
    C1,
    C2,
    C3  // Darkest
};