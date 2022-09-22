#include "MainWindow.h"
#include "GameBoyView.h"
#include "displayconst.h"
//#include "emulator.h"
#include <iostream>
#include <string>
#include <QApplication>
#include <QWidget>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    //Emulator emuboy;

    MainWindow main_window;
    main_window.setWindowTitle("EmuBoy");
    main_window.resize(qt_view_width, qt_view_height);
    main_window.show();

    GameBoyView gbview(qt_view_width, qt_view_height);
    main_window.setCentralWidget(&gbview);

    cout << "EmuBoy V0.46.0" << endl;
    gbview.start_emulator();

    // Command line: gameboy /path/to/rom.gb [-dmg] /path/to/dmg_rom.bin [-debug]
    // TODO: Not yet supported running dmg rom and game rom together
    // if (argc == 1) {
    //     cout << "Command line argument not found, expecting path to ROM file" << endl;
    //     emuboy.print_cl_usage_message();
    // } else if (argc == 2) {
    //     cout << "Loading ROM " << argv[1] << endl;
    //     emuboy.start(argv[1], false, false);
    // } else if (argc == 3) {
    //     if (strcmp(argv[2], "-debug") == 0) {
    //         cout << "Loading ROM in debug mode " << argv[1] << endl;
    //         emuboy.start(argv[1], false, true);
    //     } else {
    //         cout << "Command line argument \"" << argv[2] << "\" not expected" << endl;
    //         return 1;
    //     }
    // } else if (argc <= 5) {
    // 	if (strcmp(argv[2], "-dmg") == 0) {
    //         cout << "Loading DMG Boot Rom " << argv[1] << endl;
    //         emuboy.start(argv[3], true, false);
    // 	} else {
    // 		cout << "Unexpected command line argument" << endl;
    //         return 1;
    // 	}
    // } else {
    //     cout << "Too many command line arguments" << endl;
    //     emuboy.print_cl_usage_message();
    //     return 1;
    // }

	return app.exec();
}
