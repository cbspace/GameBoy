#include "MainWindow.h"

MainWindow::MainWindow() :
    gbview(GameBoyView(qt_view_width, qt_view_height))
{
    // auto* file_menu = menuBar()->addMenu("File");
    // auto* view_menu = menuBar()->addMenu("View");
    // auto* about_menu = menuBar()->addMenu("About");

    setCentralWidget(&gbview);

    //cout << "EmuBoy V0.46.0" << endl;
    gbview.start_emulator();


    timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, [this] {
        gbview.emulator.main_loop();
    });
    timer->setInterval(1);
    timer->start();

    timer2 = new QTimer(this);
    QObject::connect(timer2, &QTimer::timeout, [this] {
        gbview.repaint();
    });
    timer2->setInterval(50);
    timer2->start();
}