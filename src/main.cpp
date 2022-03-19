#include "main.h"
#include "emulator/emulator.h"
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

    if (argc == 1) {
        emuboy.print_error("Command line argument not found, expecting path to ROM file");
    }
    else if (argc == 2) {
            cout << "Loading ROM " << argv[1] << endl;
            emuboy.start(argv[1], false);
    } else if (argc == 3) {
    	cout << argv[2] << endl;
    	if (strcmp(argv[2], ARG_DMG) == 0) {
            cout << "Loading DMG Boot Rom" << argv[1] << endl;
            emuboy.start(argv[1], true);
    	} else {
    		emuboy.print_error("Unknown command line argument");
    	}
    } else {
        emuboy.print_error("Too many command line arguments");
    }

	return 0;
}
