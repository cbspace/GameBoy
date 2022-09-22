#include "MainWindow.h"

MainWindow::MainWindow() {
    auto* file_menu = menuBar()->addMenu("File");
    auto* view_menu = menuBar()->addMenu("View");
    auto* about_menu = menuBar()->addMenu("About");
}