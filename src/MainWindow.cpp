#include "MainWindow.h"

MainWindow::MainWindow() :
    gbview(GameBoyView(this, qt_view_width, qt_view_height))
{
    setWindowTitle("EmuBoy");
    setCentralWidget(&gbview);

    auto* file_menu = menuBar()->addMenu("File");
    auto* view_menu = menuBar()->addMenu("View");
    auto* about_menu = menuBar()->addMenu("About");

    gbview.start_emulator();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, &gbview, &GameBoyView::animate);
    timer->start(16);
}