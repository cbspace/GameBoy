#include "emulator.h"
#include "display.h"
#include "cpu.h"
#include "memory.h"
#include <iostream>
#include <string>
#include <SDL2/SDL.h>

using namespace std;

Emulator::Emulator()
{
    init(); // Initialise emulator
}

// Not needed as yet
void Emulator::init()
{

}

// Run the emulator
void Emulator::start(char* rom_path)
{
    // Initialise SDL
    printf("Loading Window\n");
    disp.init();

    // Load the rom
    mem.load_rom(rom_path);
    disp.set_title(mem.get_rom_title());
    cout << "Rom Title: " << mem.get_rom_title() << endl;

    // Delay temporarily
    SDL_Delay(4000);

}

// Function to print error message
void Emulator::print_error(string error_string) {
    printf("Error: %s\n", error_string.c_str());
}
