#include "DebugWindow.h"

DebugWindow::DebugWindow(MainWindow* parent) :
    m_parent(parent)
{
    setWindowTitle("EmuBoy Debugger");
    setFixedSize(800, 600);
}

DebugWindow::~DebugWindow() {}