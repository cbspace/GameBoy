#pragma once

#include "MainWindow.h"
#include <QWidget>
#include <QLabel>

class MainWindow;

class DebugWindow : public QWidget {
    Q_OBJECT

    public:
        DebugWindow(MainWindow* parent);
        ~DebugWindow();
        void setup_ui();
        void update_ui();
    private:
        MainWindow* m_parent { NULL };

        QLabel* label_pc { NULL };
        QLabel* value_pc { NULL };
};