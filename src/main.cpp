#include "MainWindow.h"
#include "GameBoyView.h"
#include <QApplication>

using namespace std;

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow main_window;
    main_window.show();

	return app.exec();
}
