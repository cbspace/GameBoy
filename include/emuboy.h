#ifndef EMUBOY_H
#define EMUBOY_H

#define scaling_factor 2        // Scale display size by this amount
#define window_title "EmuBoy"   // Title to display on SDL Window

#include <string>
using namespace std;

static const float RELEASE_VERSION = 0.1;
static const int width = 160*scaling_factor;    // Screen width in pixels
static const int height = 144*scaling_factor;   // Screen height in pixels

void print_error(string error_string);

#endif // EMUBOY_H

