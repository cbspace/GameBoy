#include "emuboy.h"
#include "display.h"
#include "cpu.h"
#include "memory.h"

// Program Entry Point
int main(int argc, char* argv[])
{
    // Print welcome message to console
    printf("EmuBoy V %1.2f\n", RELEASE_VERSION);

    // Initialise SDL and create display
    display myDisplay(window_title, width, height);
    printf("Loading Window\n");

    // Create the CPU
    cpu myCpu();

    // Delay temporarily
    SDL_Delay(1000);

	return 0;
}
