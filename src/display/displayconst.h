#ifndef DISPLAYCONST_H
#define DISPLAYCONST_H

#define SCALING_FACTOR      2                  // Scale display size by this amount
#define WINDOW_TITLE        "EmuBoy"           // Title to display on SDL Window

#define COLOUR_BG_ARGB      0xFF92AD26         // Background colour
#define COLOUR_C1_ARGB      0xFF6D811C         // Colour 1
#define COLOUR_C2_ARGB      0xFF485612         // Colour 2
#define COLOUR_C3_ARGB      0xFF242B09         // Colour 3

#define DISP_W   160           // Gameboy display width (Usually 160 pixels)
#define DISP_H   144           // Gameboy display height (Usually 144 pixels)

#define BG_TILES_MAX        32
#define SPRITE_TILES_MAX    32

#define LCD_Y_MAX			153 // Max Value of LY register

// Memory Locations - Video
#define A_TDT1      0x8000      // Tile Data Table 1 start address
#define A_TDT2      0x8800      // Tile Data Table 2 start address
#define A_BGTM1     0x9800      // Background Tile Map 1 start address
#define A_BGTM2     0x9C00      // Background Tile Map 2 start address

#define A_OAM       0xfe00      // Start address of OAM memory

// Special Registers - Video
#define R_LCDC      0xff40      // LCD Control
#define R_LCDSTAT   0xff41      // LCD Status
#define R_SCY       0xff42      // Scroll Y
#define R_SCX       0xff43      // Scroll X
#define R_LY        0xff44      // LCD Y coordinate
#define R_LYC       0xff45      // LY Compare
#define R_DMA       0xff46      // Direct Memory Access
#define R_BGP       0xff47      // Background Palette
#define R_OBP0      0xff48      // Object Palette 0
#define R_OBP1      0xff49      // Object Palette 1
#define R_WY        0xff4a      // Window Y Position
#define R_WX        0xff4b      // Window X Position

static const int width = DISP_W * SCALING_FACTOR;    // Screen width in pixels
static const int height = DISP_H * SCALING_FACTOR;   // Screen height in pixels

#endif // DISPLAYCONST_H
