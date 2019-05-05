#include "main.h"
#include "emulator.h"
#include <iostream>
#include <string>

using namespace std;

// Program Entry Point
int main(int argc, char* argv[])
{
    // Create an emulator object
    Emulator emuboy;

    // Print welcome message to console
    cout << "EmuBoy V " << RELEASE_VERSION << endl;

    // Load ROM from command line argument
    if (argc == 2) {
        cout << "Loading ROM " << argv[1] << endl;

        emuboy.start(argv[1]);
    } else if (argc == 1) {
        emuboy.print_error("Command line argument not found, expecting path to ROM file");
    } else {
        emuboy.print_error("Too many commnd line arguments");
    }

	return 0;
}
