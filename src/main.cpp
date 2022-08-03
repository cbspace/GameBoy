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
    // Command line: gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]
    // TODO: Not yet supported running dmg rom and game rom together
    if (argc == 1) {
        emuboy.print_error("Command line argument not found, expecting path to ROM file");
        cout << "Command line usage:" << endl;
        cout << "(1)  gameboy /path/to/rom.gb [-debug]" << endl;
        cout << "(2)  gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]" << endl;
    }
    else if (argc == 2) {
        cout << "Loading ROM " << argv[1] << endl;
        emuboy.start(argv[1], false, false);
    } else if (argc == 3) {
        if (strcmp(argv[2], ARG_DEBUG) == 0) {
            cout << "Loading ROM in debug mode " << argv[1] << endl;
            emuboy.start(argv[1], false, true);
        } else {
            cout << "Command line argument \"" << argv[2] << "\" not expected" << endl;
        }
    } else if (argc <= 5) {
    	if (strcmp(argv[2], ARG_DMG) == 0) {
            cout << "Loading DMG Boot Rom " << argv[1] << endl;
            emuboy.start(argv[3], true, false);
    	} else {
    		emuboy.print_error("Unexpected command line argument");
    	}
    } else {
        emuboy.print_error("Too many command line arguments");
    }

	return 0;
}
