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

static const int width = DISP_W * SCALING_FACTOR;    // Screen width in pixels
static const int height = DISP_H * SCALING_FACTOR;   // Screen height in pixels

#endif // DISPLAYCONST_H
