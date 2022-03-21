#ifndef DISPLAYCONST_H
#define DISPLAYCONST_H

#define SCALING_FACTOR      3                  // Scale display size by this amount
#define WINDOW_TITLE        "EmuBoy"           // Title to display on SDL Window

#define COLOUR_BG_ARGB      0xFF92AD26         // Background colour
#define COLOUR_C1_ARGB      0xFF6D811C         // Colour 1
#define COLOUR_C2_ARGB      0xFF485612         // Colour 2
#define COLOUR_C3_ARGB      0xFF242B09         // Colour 3

#define DISP_W   160            // Gameboy display width (Usually 160 pixels)
#define DISP_H   144            // Gameboy display height (Usually 144 pixels)

#define BG_TILES_MAX        32	// BG is 32 x 32 tiles
#define SPRITE_TILES_MAX    40  // Sprite OAM memory for 40 sprites
#define OAM_DATA_LENGTH		4   // OAM memory data is 4 bytes long

#define LCD_Y_MAX			153 // Max Value of LY register

// Memory Locations - Video
#define A_TDT1      0x8000      // Tile Data Table 1 start address (Sprite, BG, Window: 0->255)
#define A_TDT2      0x8800      // Tile Data Table 2 start address (BG, Window: -128->127)
#define A_BGTM1     0x9800      // Background Tile Map 1 start address
#define A_BGTM2     0x9C00      // Background Tile Map 2 start address

#define A_OAM       0xfe00      // Start address of OAM memory (Sprite Atrribute Table)

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

#define R_LCDC_LCD_CONTROL				7	// LCD Control
#define R_LCDC_WIN_MAP_SEL				6	// Window tile map select
#define R_LCDC_WIN_DISPLAY				5	// Window display on or off
#define R_LCDC_BG_WIN_TILE_DATA_SEL		4	// BG & Window tile data select
#define R_LCDC_BG_TILE_MAP_SEL			3	// BG tile map select
#define R_LCDC_SPRITE_SIZE				2	// Sprite size (8x8 or 8x16)
#define R_LCDC_SPRITE_DISPLAY			1	// Sprite display on or off
#define R_LCDC_BG_WIN_DISPLAY			0	// BG and Window display on or off

#define R_STAT_LYC_IR					6	// Interrupt Sel: LYC=LY Coincidence
#define R_STAT_MODE_10_IR				5	// Interrupt Sel: Mode 10
#define R_STAT_MODE_01_IR				4	// Interrupt Sel: Mode 01
#define R_STAT_MODE_00_IR				3	// Interrupt Sel: Mode 00
#define R_STAT_LYC_FLAG					2	// LYC=LY Coincidence Flag
#define R_STAT_MODE_B1					1	// Mode flag bit 1
#define R_STAT_MODE_B0					0	// Mode flag bit 0

static const int width = DISP_W * SCALING_FACTOR;    // Screen width in pixels
static const int height = DISP_H * SCALING_FACTOR;   // Screen height in pixels

#endif // DISPLAYCONST_H
