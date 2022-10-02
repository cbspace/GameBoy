#include "MainWindow.h"

MainWindow::MainWindow() :
    gbview(new GameBoyView(this))
{
    setWindowTitle("EmuBoy");
    setCentralWidget(gbview);

    create_actions();
    create_menus();

    m_statusbar = statusBar();
    //m_statusbar->showMessage(tr("hello"));

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, gbview, &GameBoyView::animate);

    gbview->parse_command_line();
}

void MainWindow::start_timer() {
    timer->start(16);
}

void MainWindow::stop_timer() {
    timer->stop();
}

void MainWindow::create_menus() {
    emulator_menu = menuBar()->addMenu(tr("Emulator"));
    emulator_menu->addAction(open_file_act);
    emulator_menu->addAction(open_debug_act);
    emulator_menu->addAction(stop_act);
    emulator_menu->addAction(quit_act);

    view_menu = menuBar()->addMenu(tr("View"));
    view_menu->addAction(scale1x_act);
    view_menu->addAction(scale2x_act);
    view_menu->addAction(scale3x_act);

    about_menu = menuBar()->addMenu(tr("About"));
    about_menu->addAction(about_emuboy_act);
}

void MainWindow::create_actions() {
    open_file_act = new QAction(tr("&Open ROM File"), this);
    open_file_act->setShortcuts(QKeySequence::Open);
    open_file_act->setStatusTip(tr("Open a ROM file"));
    connect(open_file_act, &QAction::triggered, this, &MainWindow::open_file);

    open_debug_act = new QAction(tr("Show &Debug Window"), this);
    //open_debug_act->setShortcuts(QKeySequence::);
    open_debug_act->setStatusTip(tr("Show Debug Window"));
    connect(open_debug_act, &QAction::triggered, this, &MainWindow::open_debug);

    stop_act = new QAction(tr("&Stop Emulation"), this);
    //stop_act->setShortcuts(QKeySequence::);
    stop_act->setStatusTip(tr("Stop Emulation"));
    connect(stop_act, &QAction::triggered, this, &MainWindow::stop);

    quit_act = new QAction(tr("&Quit"), this);
    quit_act->setShortcuts(QKeySequence::Quit);
    quit_act->setStatusTip(tr("Quit"));
    connect(quit_act, &QAction::triggered, this, &MainWindow::quit);

    scale1x_act = new QAction(tr("Scale &1x"), this);
    //scale1x_act->setShortcuts();
    scale1x_act->setStatusTip(tr("Scale view 1x"));
    connect(scale1x_act, &QAction::triggered, this, &MainWindow::scale1x);

    scale2x_act = new QAction(tr("Scale &2x"), this);
    //scale2x_act->setShortcuts();
    scale2x_act->setStatusTip(tr("Scale view 2x"));
    connect(scale2x_act, &QAction::triggered, this, &MainWindow::scale2x);

    scale3x_act = new QAction(tr("Scale &3x"), this);
    //scale3x_act->setShortcuts();
    scale3x_act->setStatusTip(tr("Scale view 3x"));
    connect(scale3x_act, &QAction::triggered, this, &MainWindow::scale3x);

    about_emuboy_act = new QAction(tr("&Emuboy"), this);
    about_emuboy_act->setStatusTip(tr("About Emuboy"));
    connect(about_emuboy_act, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event) {}

void MainWindow::file() {}

void MainWindow::open_file() {
    stop();
    QString file_name = QFileDialog::getOpenFileName(this,tr("Open ROM File"), "/home/craig/", tr("GameBoy ROMS (*.gb *.bin)"));
    gbview->start_emulator(file_name.toStdString(), false, false);
}

void MainWindow::open_debug() {
    if (!debug_window) {
        debug_window = new DebugWindow(this);
    }
    debug_window->show();
    debug_window->raise();
    debug_window->activateWindow();
}

void MainWindow::stop() {
    stop_timer();
}

void MainWindow::quit() {
    QCoreApplication::quit();
}

void MainWindow::view() {}

void MainWindow::scale1x() {
    gbview->set_scaling_factor(1);
}

void MainWindow::scale2x() {
    gbview->set_scaling_factor(2);
}

void MainWindow::scale3x() {
    gbview->set_scaling_factor(3);
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
