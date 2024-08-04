#pragma once

#include "System.h"
#include <QWidget>
#include <QLabel>

class MainWindow;

class DebugWindow : public QWidget {
    Q_OBJECT

    public:
        DebugWindow(System* system_in);
        ~DebugWindow();
        void setup_ui();
        void update_ui();
    private:
        System* system { nullptr };

        QLabel* label_pc { nullptr };
        QLabel* value_pc { nullptr };
};