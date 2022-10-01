#include "MainWindow.h"

MainWindow::MainWindow() :
    gbview(GameBoyView(this, qt_view_width, qt_view_height))
{
    setWindowTitle("EmuBoy");
    setCentralWidget(&gbview);

    create_actions();
    create_menus();
    //statusBar->showMessage("hello");

    gbview.start_emulator();

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, &gbview, &GameBoyView::animate);
    timer->start(16);
}

void MainWindow::create_menus() {
    file_menu = menuBar()->addMenu("File");
    file_menu->addAction(open_act);

    view_menu = menuBar()->addMenu("View");
    view_menu->addAction(scale1x_act);
    view_menu->addAction(scale2x_act);

    about_menu = menuBar()->addMenu("About");
    about_menu->addAction(about_emuboy_act);
}

void MainWindow::create_actions() {
    open_act = new QAction(tr("&Open"), this);
    open_act->setShortcuts(QKeySequence::Open);
    open_act->setStatusTip(tr("Open a file"));
    connect(open_act, &QAction::triggered, this, &MainWindow::open);

    scale1x_act = new QAction(tr("&Scale 1x"), this);
    //scale1x_act->setShortcuts();
    scale1x_act->setStatusTip(tr("Scale view 1x"));
    connect(scale1x_act, &QAction::triggered, this, &MainWindow::scale1x);

    scale2x_act = new QAction(tr("&Scale 2x"), this);
    //scale2x_act->setShortcuts();
    scale2x_act->setStatusTip(tr("Scale view 2x"));
    connect(scale2x_act, &QAction::triggered, this, &MainWindow::scale2x);

    about_emuboy_act = new QAction(tr("&Emuboy"), this);
    about_emuboy_act->setStatusTip(tr("About Emuboy"));
    connect(about_emuboy_act, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event) {}

void MainWindow::file() {}

void MainWindow::open() {}

void MainWindow::view() {}

void MainWindow::scale1x() {}

void MainWindow::scale2x() {}

void MainWindow::about() {
    if (!about_widget) {
        about_widget = new QWidget(this);
        about_widget->setWindowTitle("About Emuboy");
        about_widget->setFixedSize(400,200);
        about_widget->setWindowFlags(Qt::Window);
    }
    about_widget->show();
    about_widget->raise();
    about_widget->activateWindow();
}
