#include "emuboy.h"
#include "display.h"
#include "cpu.h"
#include "memory.h"
#include <string>
//#include <cstdlib>
using namespace std;

// Program Entry Point
int main(int argc, char* argv[])
{
    // Print welcome message to console
    printf("EmuBoy V %1.2f\n", RELEASE_VERSION);

    // Load ROM from command line argument
    if (argc == 2) {
        printf("Loading ROM %s\n", argv[1]);
        memory mem = new memory
    } else if (argc == 1) {
        print_error("Command line argument not found, expecting path to ROM file");
    } else {
        print_error("Too many commnd line arguments");
    }

    // Initialise SDL and create display
    display myDisplay(window_title, width, height);
    printf("Loading Window\n");

    // Create the CPU
    cpu myCpu();

    // Delay temporarily
    SDL_Delay(5000);

	return 0;
}

void start() {

}

// Function to print error message
void print_error(string error_string) {
    printf("Error: %s\n", error_string.c_str());
}
