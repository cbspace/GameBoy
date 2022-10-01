#include "MainWindow.h"

MainWindow::MainWindow() :
    gbview(GameBoyView(this))
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
    view_menu->addAction(scale3x_act);

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

    scale3x_act = new QAction(tr("&Scale 3x"), this);
    //scale3x_act->setShortcuts();
    scale3x_act->setStatusTip(tr("Scale view 3x"));
    connect(scale3x_act, &QAction::triggered, this, &MainWindow::scale3x);

    about_emuboy_act = new QAction(tr("&Emuboy"), this);
    about_emuboy_act->setStatusTip(tr("About Emuboy"));
    connect(about_emuboy_act, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event) {}

void MainWindow::file() {}

void MainWindow::open() {}

void MainWindow::view() {}

void MainWindow::scale1x() {
    gbview.set_scaling_factor(1);
}

void MainWindow::scale2x() {
    gbview.set_scaling_factor(2);
}

void MainWindow::scale3x() {
    gbview.set_scaling_factor(3);
}

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
