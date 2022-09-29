#include "MainWindow.h"
#include "GameBoyView.h"
#include "displayconst.h"
#include <iostream>
#include <QApplication>
#include <QWidget>

using namespace std;

int main(int argc, char* argv[])
{
    if (argc == 1) {
        cout << "Command line argument not found, expecting path to ROM file" << endl;
        return 1;
    }

    QApplication app(argc, argv);
    MainWindow main_window;
    main_window.show();

	return app.exec();
}
