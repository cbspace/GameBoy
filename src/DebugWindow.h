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
        MainWindow* m_parent { nullptr };

        QLabel* label_pc { nullptr };
        QLabel* value_pc { nullptr };
};