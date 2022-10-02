#pragma once

#include "MainWindow.h"
#include <QWidget>

class MainWindow;

class DebugWindow : public QWidget {
    Q_OBJECT

    public:
        DebugWindow(MainWindow* parent);
        ~DebugWindow();
    private:
        MainWindow* m_parent { NULL };
};