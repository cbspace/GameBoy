#include "emulator.h"
#include "display.h"
#include "cpu.h"
#include "memory.h"
#include <SDL2/SDL.h>

Emulator::Emulator()
{
    init(); // Initialise emulator
}

void Emulator::init()
{
    // Create display
    disp = new Display();

    // Create the CPU
    cp = new Cpu();

    // Create the memory
    mem = new Memory();
}

// Run the emulator
void Emulator::start(char* rom_path)
{
    // Initialise SDL
    disp->init();
    printf("Loading Window\n");

    // Load the rom
    mem->load_rom(rom_path);

    // Delay temporarily
    SDL_Delay(3000);
    SDL_Delay(5000);
}

// Function to print error message
void Emulator::print_error(string error_string) {
    printf("Error: %s\n", error_string.c_str());
}
