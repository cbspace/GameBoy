#include "DebugWindow.h"

DebugWindow::DebugWindow(System* system_in) :
    system(system_in)
{
    setWindowTitle("EmuBoy Debugger");
    setFixedSize(800, 600);
    setup_ui();
    update_ui(); //
}

DebugWindow::~DebugWindow() {}

void DebugWindow::setup_ui() {
    label_pc = new QLabel(this);
    label_pc->setText(QString::fromUtf8("PC:"));
    label_pc->setGeometry(QRect(30, 30, 58, 18));

    value_pc = new QLabel(this);
    value_pc->setGeometry(QRect(80, 30, 58, 18));
}

void DebugWindow::update_ui() {
    u16 pc = system->debug.mem.get_pc();
    value_pc->setText("0x" + QString::number(pc, 16));
}