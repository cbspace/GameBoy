#include "emuboy.h"
#include "display.h"
#include "cpu.h"
#include "memory.h"

int main(int argc, char* argv[])
{

    // Initialise SDL and create display
    display myDisplay(window_title, width, height);

    // Create the CPU
    cpu myCpu();

    SDL_Delay(3000);

	return 0;
}
